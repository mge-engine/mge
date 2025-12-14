// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/asset.hpp"
#include "mge/asset/asset_access.hpp"
#include "mge/asset/asset_handler.hpp"
#include "mge/asset/asset_not_found.hpp"
#include "mge/asset/asset_source.hpp"
#include "mge/core/configuration.hpp"
#include "mge/core/executable_name.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/trace.hpp"

#include <map>
#include <set>
#include <string>

#include <magic.h>

namespace mge {
    MGE_DEFINE_TRACE(ASSET);

    // Asset configuration is in the executable configuration
    // under the "asset" section.
    // The configuration is a list of repositories, each with
    // a type, a mount point and a possible mode
    // Example:
    // {
    //    "asset": {
    //        "repositories": [
    //            {
    //                "type": "file",
    //                "mount_point": "/",
    //                "mode": "r",
    //                "directory": "."
    //            },
    //            {
    //                "type": "file",
    //                "mount_point": "/assets"
    //                "mode": "rw"
    //                "directory": "assets",
    //            },
    //            {
    //                "type": "zip",
    //                "mount_point": "/zipassets",
    //                "file": "assets.zip"
    //            }
    //        ]
    //    }
    // }
    MGE_DEFINE_PARAMETER((std::vector<std::map<std::string, std::string>>),
                         asset,
                         repositories,
                         "Asset configuration");

    class mount_table
    {
    public:
        mount_table()
        {
            MGE_PARAMETER(asset, repositories).set_change_handler([this]() {
                configure();
            });
        }

        ~mount_table() {}

        asset_access_ref resolve(const mge::path&              p,
                                 asset_handler::operation_type op) const
        {
            if (m_mounts.empty()) {
                return {};
            }
            auto it = m_mounts.lower_bound(p);
            if (it == m_mounts.begin()) {
                return {};
            }
            --it;

            // Check mode compatibility
            if (op == asset_handler::operation_type::LOAD &&
                it->second.mode == asset_source::access_mode::WRITE) {
                return {};
            }
            if (op == asset_handler::operation_type::STORE &&
                it->second.mode == asset_source::access_mode::READ) {
                return {};
            }

            return it->second.factory->access(p);
        }

        void mount(const mge::path&          mount_point,
                   const std::string&        type,
                   asset_source::access_mode mode,
                   const ::mge::properties&  properties)
        {
            MGE_DEBUG_TRACE(ASSET,
                            "Mounting {} asset source at {}",
                            type,
                            mount_point.string());
            auto factory = component<asset_source>::create(type);
            if (!factory) {
                MGE_THROW(illegal_state)
                    << "Invalid mount point type for mount point '"
                    << mount_point << "' : " << type;
            }
            factory->configure(properties);
            factory->set_mount_point(mount_point);
            factory->set_mode(mode);
            m_mounts[mount_point] = {.factory = factory,
                                     .type = type,
                                     .mode = mode,
                                     .mount_point = mount_point,
                                     .properties = properties};
        }

        void umount(const mge::path& mount_point)
        {
            MGE_DEBUG_TRACE(ASSET, "Unmounting {}", mount_point.string());
            auto it = m_mounts.find(mount_point);
            if (it != m_mounts.end()) {
                m_mounts.erase(it);
            }
        }

    private:
        void configure();

        struct mount_info
        {
            asset_source_ref          factory;
            std::string               type;
            asset_source::access_mode mode;
            mge::path                 mount_point;
            ::mge::properties         properties;
        };

        std::map<mge::path, mount_info> m_mounts;
    };

    void mount_table::configure()
    {
        MGE_DEBUG_TRACE(ASSET, "Configuring mounted assets");
        std::map<path, std::string> mount_types;
        std::map<path, properties>  mount_properties;

        const auto& entries = MGE_PARAMETER(asset, repositories).get();
        for (const auto& e : entries) {
            if (e.find("mount_point") == e.end()) {
                MGE_ERROR_TRACE(ASSET, "Repository entry without mount point");
                continue;
            }
            if (e.find("type") == e.end()) {
                MGE_ERROR_TRACE(ASSET, "Repository entry without type");
                continue;
            }
            path mount_point(e.find("mount_point")->second);
            mount_types[mount_point] = e.find("type")->second;
            properties& mp = mount_properties[mount_point];
            for (const auto& [key, value] : e) {
                if (key != "mount_point" && key != "type") {
                    mp.put(key, value);
                }
            }
        }
        std::map<mge::path, mount_info> new_mounts;
        for (const auto& [mount_point, type] : mount_types) {
            MGE_DEBUG_TRACE(ASSET,
                            "Mounting {} asset source at {}",
                            type,
                            mount_point.string());
            auto it = m_mounts.find(mount_point);
            if (it != m_mounts.end() && it->second.type == type &&
                it->second.properties == mount_properties[mount_point]) {
                new_mounts[mount_point] = it->second;
                continue;
            }
            auto factory = component<asset_source>::create(type);
            if (!factory) {
                MGE_THROW(illegal_state)
                    << "Invalid mount point type for mount point '"
                    << mount_point.string() << "' : " << type;
            }
            factory->configure(mount_properties[mount_point]);
            factory->set_mount_point(mount_point);
            new_mounts[mount_point] = {.factory = factory,
                                       .type = type,
                                       .mode = asset_source::access_mode::READ,
                                       .mount_point = mount_point,
                                       .properties =
                                           mount_properties[mount_point]};
        }
        m_mounts.swap(new_mounts);
    }

    static ::mge::singleton<mount_table> mtab;

    class handler_table
    {
    public:
        handler_table()
        {
            instantiate_handlers();
        }
        ~handler_table() = default;

        asset_handler_ref resolve(const asset_type&             t,
                                  asset_handler::operation_type op =
                                      asset_handler::operation_type::LOAD) const
        {
            auto& handlers = (op == asset_handler::operation_type::STORE)
                                 ? m_store_handlers
                                 : m_load_handlers;
            auto  it = handlers.find(t);
            if (it != handlers.end()) {
                return it->second;
            }
            refresh_handlers();
            it = handlers.find(t);
            return (it != handlers.end()) ? it->second : asset_handler_ref{};
        }

        asset_type improve_type(const asset&      a,
                                const asset_type& type,
                                int               recursion_depth = 0) const
        {
            if (recursion_depth > 10) {
                MGE_THROW(illegal_state) << "Recursion depth exceeded while "
                                            "improving asset type for "
                                         << a.path().string();
            }

            for (const auto& h : m_all_handlers) {
                if (h->can_improve(a, type)) {
                    auto t = h->improve(a, type);
                    if (t != asset_type::UNKNOWN) {
                        return t;
                    }
                }
            }
            if (refresh_handlers()) {
                return improve_type(a, type, recursion_depth + 1);
            }
            return type;
        }

        void add_handler(const asset_handler_ref& handler) const
        {
            if (!handler) {
                return;
            }
            m_all_handlers.insert(handler);
            for (const auto& t :
                 handler->handled_types(asset_handler::operation_type::LOAD)) {
                MGE_DEBUG_TRACE(ASSET,
                                "Adding load handler for asset type: {}",
                                t);
                m_load_handlers[t] = handler;
            }
            for (const auto& t :
                 handler->handled_types(asset_handler::operation_type::STORE)) {
                MGE_DEBUG_TRACE(ASSET,
                                "Adding store handler for asset type: {}",
                                t);
                m_store_handlers[t] = handler;
            }
        }

    private:
        void instantiate_handlers();
        bool refresh_handlers() const;

        mutable std::set<std::string, std::less<>>      m_handler_names;
        mutable std::set<asset_handler_ref>             m_all_handlers;
        mutable std::map<asset_type, asset_handler_ref> m_load_handlers;
        mutable std::map<asset_type, asset_handler_ref> m_store_handlers;
    };

    void handler_table::instantiate_handlers()
    {
        asset_handler::implementations([&](std::string_view name) {
            MGE_DEBUG_TRACE(ASSET, "Instantiating asset handler: {}", name);
            asset_handler_ref handler = asset_handler::create(name);
            add_handler(handler);
            m_handler_names.insert(std::string(name));
        });
    }

    bool handler_table::refresh_handlers() const
    {
        bool handler_changed = false;
        asset_handler::implementations([&](std::string_view name) {
            auto it = m_handler_names.find(name);
            if (it == m_handler_names.end()) {
                MGE_DEBUG_TRACE(ASSET, "Instantiating asset handler: {}", name);
                asset_handler_ref handler = asset_handler::create(name);
                add_handler(handler);
                m_handler_names.insert(std::string(name));
                handler_changed = true;
            }
        });
        return handler_changed;
    }

    static ::mge::singleton<handler_table> handlers;

    bool asset::exists() const
    {
        return m_access || resolve();
    }

    bool asset::exists(const mge::path& p)
    {
        asset tmp(p);
        return tmp.exists();
    }

    size_t asset::size() const
    {
        if (!m_access) {
            if (!resolve()) {
                MGE_THROW(asset_not_found)
                    << "Asset not found: " << m_path.string();
            }
        }
        return m_access->size();
    }

    mge::input_stream_ref asset::data() const
    {
        if (!m_access) {
            if (!resolve()) {
                MGE_THROW(asset_not_found)
                    << "Asset not found: " << m_path.string();
            }
        }
        return m_access->data();
    }

    bool asset::resolve() const
    {
        m_access = mtab->resolve(m_path, asset_handler::operation_type::LOAD);
        return m_access.operator bool();
    }

    asset_type asset::type() const
    {
        if (m_type.has_value()) {
            return m_type.value();
        }

        if (!m_access) {
            if (!resolve()) {
                MGE_THROW(asset_not_found)
                    << "Asset not found: " << m_path.string();
            }
        }
        m_type = m_access->type();
        if (m_type.value() == asset_type::UNKNOWN) {
            m_type = magic();
        }

        // somewhat hacky: try to specifialize the type
        // based on the file extension
        if (m_type.value() == asset_type::UNKNOWN ||
            m_type.value() == asset_type("text", "plain")) {
            m_type = handlers->improve_type(*this, m_type.value());
        }

        return m_type.value();
    }

    std::any asset::load() const
    {
        // will resolve (and throw if asset not found)
        auto t = type();
        auto h = handlers->resolve(t);
        if (!h) {
            MGE_THROW(illegal_state) << "No load handler for asset type '" << t
                                     << "' for asset: " << m_path.string();
        }
        return h->load(*this);
    }

    void asset::store(const asset_type& type, const std::any& asset) const
    {
        auto h = handlers->resolve(type, asset_handler::operation_type::STORE);
        if (!h) {
            MGE_THROW(illegal_state)
                << "No store handler for asset type '" << type
                << "' for asset: " << m_path.string();
        }
        h->store(*this, type, asset);
    }

    class magican
    {
    public:
        magican();
        ~magican();

        asset_type magic(const void* buffer, size_t size) const;

    private:
        magic_t m_magic;
    };

    magican::magican()
        : m_magic(magic_open(MAGIC_MIME_TYPE))
    {
        if (m_magic == nullptr) {
            MGE_THROW(runtime_exception) << "Cannot open magic database";
        }

        std::string magic_file = mge::executable_path();
#ifdef MGE_OS_WINDOWS
        magic_file += "\\mge_magic.mgc";
#else
        magic_file += "/mge_magic.mgc";
#endif

        if (magic_load(m_magic, magic_file.c_str()) != 0) {
            std::string err = magic_error(m_magic);
            magic_close(m_magic);
            m_magic = nullptr;
            MGE_THROW(runtime_exception)
                << "Cannot load magic database: " << err;
        }
    }

    magican::~magican()
    {
        if (m_magic) {
            magic_close(m_magic);
            m_magic = nullptr;
        }
    }

    asset_type magican::magic(const void* buffer, size_t size) const
    {
        const char* mf = magic_buffer(m_magic, buffer, size);
        if (mf == nullptr) {
            std::string err = magic_error(m_magic);
            MGE_THROW(runtime_exception)
                << "Cannot determine asset type: " << err;
        }
        asset_type result = asset_type::parse(mf);
        return result;
    }

    static mge::singleton<magican> s_magican;

    asset_type asset::magic() const
    {
        MGE_DEBUG_TRACE(ASSET, "Determining asset type using magic");
        char buffer[1024];

        mge::input_stream::streamsize_type buffersize =
            std::min(m_access->size(), sizeof(buffer));

        auto pos = m_access->data()->position();
        m_access->data()->read(buffer, buffersize);
        m_access->data()->seek(pos, mge::input_stream::POS_BEG);
        return s_magican->magic(buffer, buffersize);
    }

    void asset::mount(const mge::path&          mount_point,
                      const std::string&        type,
                      asset_source::access_mode mode,
                      const ::mge::properties&  options)
    {
        mtab->mount(mount_point, type, mode, options);
    }

    void asset::mount(const mge::path&         mount_point,
                      const std::string&       type,
                      const ::mge::properties& options)
    {
        mtab->mount(mount_point,
                    type,
                    asset_source::access_mode::READ,
                    options);
    }

    void asset::umount(const mge::path& path)
    {
        mtab->umount(path);
    }

} // namespace mge
