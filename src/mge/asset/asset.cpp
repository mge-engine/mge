// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/asset.hpp"
#include "mge/asset/asset_access.hpp"
#include "mge/asset/asset_access_factory.hpp"
#include "mge/asset/asset_locator.hpp"
#include "mge/asset/asset_not_found.hpp"
#include "mge/core/configuration.hpp"
#include "mge/core/log.hpp"
#include "mge/core/singleton.hpp"
#include <map>

MGE_DEFINE_LOG(ASSET)

namespace mge {

    class asset_repository_manager
    {
    public:
        asset_repository_manager() : m_configured(false) { try_configure(); }

        asset_access_ref access(const mge::path &path)
        {
            mge::path asset_path(path);
            if (!asset_path.is_absolute()) {
                asset_path = mge::path("/") / asset_path;
            }

            if (!m_configured) {
                configure();
            }

            auto lb = m_mtab.lower_bound(asset_path);
            if (lb == m_mtab.begin()) {
                MGE_THROW(mge::no_such_element)
                    << "Cannot resolve asset '" << asset_path << "'";
            }
            auto factory = (--lb)->second;
            return factory->create_asset_access(asset_path);
        }

        void configure()
        {
            MGE_DEBUG_LOG(ASSET) << "Configuring asset repository";

            mge::configuration asset_config("asset");
            if (asset_config.contains_key("repositories")) {
                for (const auto &repo :
                     asset_config.list_value("repositories")) {
                    std::string config_name("asset.repository.");
                    config_name += repo;
                    configuration repo_config(config_name);
                    std::string   factory_class = repo_config.value("class");
                    auto factory = asset_access_factory::create(factory_class);
                    if (!factory) {
                        continue;
                    }
                    factory->configure(repo_config);
                    mge::path mount_point = repo_config.value("mount_point");
                    factory->set_mountpoint(mount_point);
                    m_mtab[mount_point] = factory;
                }
            }
            mge::path default_path("assets/");
            mge::path default_mount_point("/");
            if (m_mtab.find(default_mount_point) == m_mtab.end()) {
                mge::configuration default_config(
                    mge::configuration::transient);
                default_config.set("directory", default_path.string());
                auto default_factory = asset_access_factory::create("file");
                default_factory->configure(default_config);
                default_factory->set_mountpoint(default_mount_point);
                m_mtab[default_mount_point] = default_factory;
            }
            MGE_DEBUG_LOG(ASSET) << "Asset mount table:";
            MGE_DEBUG_LOG(ASSET) << mge::line(60);
            for (const auto &[key, value] : m_mtab) {
                MGE_DEBUG_LOG(ASSET) << key << " => " << gist(*value);
            }
            MGE_DEBUG_LOG(ASSET) << mge::line(60);
            m_configured = true;
        }

    private:
        void try_configure()
        {
            try {
                configure();
            } catch (const mge::exception &) {
                // ignore
            }
        }
        bool                                          m_configured;
        std::map<mge::path, asset_access_factory_ref> m_mtab;
    };
    static singleton<asset_repository_manager> repository_manager;

    size_t asset::size() const
    {
        if (!m_access) {
            m_access = repository_manager->access(m_path);
        }
        return m_access->size();
    }

    mge::input_stream_ref asset::data() const
    {
        if (!m_access) {
            m_access = repository_manager->access(m_path);
        }
        return m_access->data();
    }

    asset_type asset::type() const
    {
        if (!m_access) {
            m_access = repository_manager->access(m_path);
        }
        return m_access->type();
    }

    bool asset::has_properties() const
    {
        if (!m_access) {
            m_access = repository_manager->access(m_path);
        }
        return m_access->has_properties();
    }

    properties_ref asset::properties() const
    {
        if (!m_access) {
            m_access = repository_manager->access(m_path);
        }
        return m_access->properties();
    }

    void asset::gist(std::ostream &os) const
    {
        os << "asset[" << path() << "]";
    }

    namespace {
        class locator_registry
        {
        public:
            mge::path locate(const std::string &name, const asset_type &type)
            {
                mge::path result;
                for (const auto &[locator_name, locator] : m_locators) {
                }
                return mge::path();
            }

        private:
            std::map<std::string, asset_locator_ref> m_locators;
        };

        static mge::singleton<locator_registry> s_locator_registry;
    } // namespace

    asset asset::locate(const std::string &name, const asset_type &type)
    {
        auto p = s_locator_registry->locate(name, type);
        if (p.empty()) {
            MGE_THROW(asset_not_found)
                << "Asset with name '" << name << "' could not be looked up";
        } else {
            return asset(p);
        }
    }

    namespace string_literals {
        asset operator""_asset(const char *s, size_t sz)
        {
            return asset(std::string(s, s + sz));
        }
    } // namespace string_literals
} // namespace mge
