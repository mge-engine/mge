#include "mge/asset/asset_loader.hpp"
#include "mge/asset/asset.hpp"
#include "mge/core/singleton.hpp"
#include <map>
namespace mge {
    MGE_REGISTER_COMPONENT(asset_loader);

    class asset_loader_collection
    {
    public:
        asset_loader_collection() = default;
        ~asset_loader_collection() = default;

        asset_loader_ref find_loader(const asset_type& t)
        {
            auto it = m_loaders.find(t);
            if (it != m_loaders.end()) {
                return it->second;
            }
            return asset_loader_ref();
        }
    private:
        std::map<asset_type, asset_loader_ref> m_loaders;
    };

    singleton<asset_loader_collection> s_loaders;

    asset_loader_ref
    asset_loader::loader(const asset& a)
    {
        if(a.type() == asset_type::UNKNOWN) {
            return asset_loader_ref();
        }
        return s_loaders->find_loader(a.type());
    }
}
