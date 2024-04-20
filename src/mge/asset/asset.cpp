// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/asset.hpp"
#include "mge/asset/asset_access.hpp"
#include "mge/asset/asset_loader.hpp"
#include "mge/asset/asset_not_found.hpp"
#include "mge/asset/asset_source.hpp"
#include "mge/core/configuration.hpp"
#include "mge/core/executable_name.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/trace.hpp"

#include <map>
#include <string>

#include <magic.h>

namespace mge {
    MGE_DEFINE_TRACE(ASSET);

    // Asset configuration is in the executable configuration
    // under the "asset" section.
    // The configuration is a list of repositories, each with
    // a type and a mount point.
    // Example:
    // {
    //    "asset": {
    //        "repositories": [
    //            {
    //                "type": "file",
    //                "mount_point": "/",
    //                "directory": "."
    //            },
    //            {
    //                "type": "file",
    //                "mount_point": "/assets"
    //                "directory": "assets"
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

        asset_access_ref resolve(const mge::path& p) const
        {
            if (m_mounts.empty()) {
                return asset_access_ref();
            } else {
                auto it = m_mounts.lower_bound(p);
                if (it == m_mounts.begin()) {
                    return asset_access_ref();
                } else {
                    --it;
                    return it->second.factory->access(p);
                }
            }

            return asset_access_ref();
        }

        void mount(const mge::path&         mount_point,
                   const std::string&       type,
                   const ::mge::properties& options)
        {
            MGE_DEBUG_TRACE(ASSET)
                << "Mounting " << type << " asset source at " << mount_point;
            mount_info mi;
            mi.mount_point = mount_point;
            mi.type = type;
            mi.properties = options;
            mi.factory = component<asset_source>::create(type);
            if (!mi.factory) {
                MGE_THROW(illegal_state)
                    << "Invalid mount point type for mount point '"
                    << mount_point << "' : " << type;
            } else {
                mi.factory->configure(mi.properties);
                mi.factory->set_mount_point(mount_point);
                m_mounts[mount_point] = mi;
            }
        }

        void umount(const mge::path& mount_point)
        {
            MGE_DEBUG_TRACE(ASSET) << "Unmounting " << mount_point;
            auto it = m_mounts.find(mount_point);
            if (it != m_mounts.end()) {
                m_mounts.erase(it);
            }
        }

    private:
        void configure();

        struct mount_info
        {
            asset_source_ref  factory;
            std::string       type;
            mge::path         mount_point;
            ::mge::properties properties;
        };

        std::map<mge::path, mount_info> m_mounts;
    };

    void mount_table::configure()
    {
        MGE_DEBUG_TRACE(ASSET) << "Configuring mounted assets";
        std::map<path, std::string> mount_types;
        std::map<path, properties>  mount_properties;

        const auto& entries = MGE_PARAMETER(asset, repositories).get();
        for (const auto& e : entries) {
            if (e.find("mount_point") == e.end()) {
                MGE_ERROR_TRACE(ASSET)
                    << "Repository entry without mount point";
            } else if (e.find("type") == e.end()) {
                MGE_ERROR_TRACE(ASSET) << "Repository entry without type";
            } else {
                path mount_point(e.find("mount_point")->second);
                mount_types[mount_point] = e.find("type")->second;
                properties& mp = mount_properties[mount_point];
                for (const auto [key, value] : e) {
                    if (key != "mount_point" && key != "type") {
                        mp.put(key, value);
                    }
                }
            }
        }
        std::map<mge::path, mount_info> new_mounts;
        for (const auto& [mount_point, type] : mount_types) {
            MGE_DEBUG_TRACE(ASSET)
                << "Mounting " << type << " asset source at " << mount_point;
            auto it = m_mounts.find(mount_point);
            if (it != m_mounts.end()) {
                if (it->second.type == type) {
                    if (it->second.properties ==
                        mount_properties[mount_point]) {
                        new_mounts[mount_point] = it->second;
                        continue;
                    }
                }
            }
            mount_info mi;
            mi.mount_point = mge::path(mount_point);
            mi.type = type;
            mi.properties = mount_properties[mount_point];
            mi.factory = component<asset_source>::create(type);
            if (!mi.factory) {
                MGE_THROW(illegal_state)
                    << "Invalid mount point type for mount point '"
                    << mount_point << "' : " << type;
            } else {
                mi.factory->configure(mi.properties);
                mi.factory->set_mount_point(mount_point);
                new_mounts[mount_point] = mi;
            }
        }
        m_mounts.swap(new_mounts);
    }

    static ::mge::singleton<mount_table> mtab;

    class loader_table
    {
    public:
        loader_table() { instantiate_loaders(); }
        ~loader_table() = default;

        asset_loader_ref resolve(const asset_type& t) const
        {
            auto it = m_loaders.find(t);
            if (it == m_loaders.end()) {
                return asset_loader_ref();
            } else {
                return it->second;
            }
        }

        void add_loader(const asset_loader_ref& loader)
        {
            if (loader) {
                for (const auto& t : loader->handled_types()) {
                    m_loaders[t] = loader;
                }
            }
        }

    private:
        void instantiate_loaders();

        std::map<asset_type, asset_loader_ref> m_loaders;
    };

    void loader_table::instantiate_loaders()
    {
        asset_loader::implementations([&](std::string_view name) {
            asset_loader_ref loader = asset_loader::create(name);
            add_loader(loader);
        });
    }

    static ::mge::singleton<loader_table> loaders;

    bool asset::exists() const { return m_access || resolve(); }

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

    bool asset::resolve() const
    {
        m_access = mtab->resolve(m_path);
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
        return m_type.value();
    }

    std::any asset::load() const
    {
        // will resolve (and throw if asset not found)
        auto t = type();
        auto l = loaders->resolve(t);
        if (!l) {
            MGE_THROW(illegal_state) << "No loader for asset type '" << t
                                     << "' for asset: " << m_path.string();
        }
        return l->load(*this);
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
        magic_file += "\\mge_magic.mgc";

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
        MGE_DEBUG_TRACE(ASSET) << "Determining asset type using magic";
        char buffer[1024];

        mge::input_stream::streamsize_type buffersize =
            std::min(m_access->size(), sizeof(buffer));

        auto pos = m_access->data()->position();
        m_access->data()->read(buffer, buffersize);
        m_access->data()->seek(pos, mge::input_stream::POS_BEG);
        return s_magican->magic(buffer, buffersize);
    }

    void asset::mount(const mge::path&         mount_point,
                      const std::string&       type,
                      const ::mge::properties& options)
    {
        mtab->mount(mount_point, type, options);
    }

    void asset::umount(const mge::path& path) { mtab->umount(path); }

} // namespace mge
