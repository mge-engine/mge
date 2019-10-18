// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/asset.hpp"
#include "mge/core/singleton.hpp"
#include "mge/asset/asset_access.hpp"
#include "mge/asset/asset_access_factory.hpp"
#include "mge/core/log.hpp"
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
            asset_access_ref result;
            return result;
        }

        void configure()
        {
            MGE_DEBUG_LOG(ASSET) << "Configuring asset repository";
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