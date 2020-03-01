#include "mge/asset/asset_loader.hpp"
#include "mge/asset/asset.hpp"
#include "mge/core/singleton.hpp"
#include <map>
#include <set>

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
            } else {
                auto updated = update_loaders();
                if (updated) {
                    return find_loader(t);
                }
            }
            return asset_loader_ref();
        }
    private:
        bool update_loaders()
        {
            std::vector<std::string> new_implementations;
            asset_loader::implementations([&](const std::string& name){
                if(m_loader_implementations.find(name) == m_loader_implementations.end()) {
                    new_implementations.emplace_back(name);
                }
            });
            if (new_implementations.empty()) {
                return false;
            }
            for(const auto& name : new_implementations) {
                add_loader(name);
            }
            return true;
        }

        void add_loader(const std::string& name)
        {
            auto loader = asset_loader::create(name);
            std::vector<asset_type> handled_types;
            loader->handled_types([&](const asset_type& t){
                handled_types.emplace_back(t);
            });
            m_loader_implementations.insert(name);
            for(const auto& t : handled_types) {
                m_loaders[t] = loader;
            }
        }

        std::set<std::string> m_loader_implementations;
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
