// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/asset.hpp"
#include "mge/core/singleton.hpp"
#include "mge/asset/asset_access.hpp"
#include "mge/asset/asset_access_factory.hpp"
#include "mge/core/log.hpp"
#include "mge/core/configuration.hpp"
#include <map>

MGE_DEFINE_LOG(ASSET)

namespace mge {

    class asset_repository_manager
    {
    public:
        asset_repository_manager()
            :m_configured(false)
        {
            try_configure();
        }

        asset_access_ref access(const mge::path path)
        {
            if(!m_configured) {
                configure();
            }

            auto lb = m_mtab.lower_bound(path);
            if (lb == m_mtab.end()) {
                MGE_THROW(mge::runtime_exception) << "Cannot resolve asset " << path;
            }
            std::cout << "lb: " << lb->first << std::endl;

            asset_access_ref result;
            return result;
        }

        void configure()
        {
            MGE_DEBUG_LOG(ASSET) << "Configuring asset repository";

            mge::configuration asset_config("asset");
            if (asset_config.contains_key("repositories")) {
                for (const auto& repo : asset_config.list_value("repositories")) {
                    std::string config_name("asset.repository.");
                    config_name += repo;
                    configuration repo_config(config_name);
                    std::string factory_class = repo_config.value("class");
                    auto factory = asset_access_factory::create(factory_class);
                    if (!factory) {
                        continue;
                    }
                    factory->configure(repo_config);
                    mge::path mount_point = repo_config.value("mount_point");
                    m_mtab[mount_point] = factory;
                }
            }
            mge::path default_path("assets/");
            if (m_mtab.find(default_path) == m_mtab.end()) {
                mge::configuration default_config(mge::configuration::transient);
                default_config.set("directory", default_path.string());
                auto default_factory = asset_access_factory::create("file");
                default_factory->configure(default_config);
                m_mtab[default_path] = default_factory;
            }
#if 0
            for (const auto& [key, value] : m_mtab) {
                MGE_DEBUG_LOG(ASSET) << key << " => " << value;
            }
#endif
            m_configured = true;
        }

    private:
        void try_configure()
        {
            try {
                configure();
            } catch(const mge::exception&) {
                // ignore
            }
        }
        bool m_configured;
        std::map<mge::path, asset_access_factory_ref> m_mtab;
    };
    static singleton<asset_repository_manager> repository_manager;    


    size_t
    asset::size() const
    {
        if(!m_access) {
            m_access = repository_manager->access(m_path);
        }
        return m_access->size();
    }

    mge::input_stream_ref
    asset::data() const
    {
        if(!m_access) {
            m_access = repository_manager->access(m_path);
        }
        return m_access->data();
    }

    asset_type
    asset::type() const
    {
        if(!m_access) {
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

    void
    asset::gist(std::ostream& os) const
    {
        os << "asset[" << path() << "]";
    }

    namespace string_literals {
        asset operator ""_asset(const char *s, size_t sz)
        {
            return asset(std::string(s, s+sz));
        }
    }
}