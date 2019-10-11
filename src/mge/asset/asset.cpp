#include "mge/asset/asset.hpp"
#include "mge/core/singleton.hpp"
#include "mge/asset/asset_access.hpp"

namespace mge {

    class asset_repository_manager
    {
    public:
        asset_access_ref access(const mge::path path)
        {
            asset_access_ref result;
            return result;
        }
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

    asset operator ""_asset(const char *s, size_t sz)
    {
        return asset(std::string(s, s+sz));
    }
}