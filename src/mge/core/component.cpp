// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/component.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/contains.hpp"
#include "mge/core/stdexceptions.hpp"

#include <map>
#include <string>
#include <iostream>
#include <vector>

#include "boost/algorithm/string.hpp"

namespace mge {

    class component_registry
    {
    public:
        typedef std::map<std::string, component_registry_entry_base *> component_map_t;
        typedef std::map<std::string, std::map<std::string, implementation_registry_entry_base*>> implementation_map_t;
        typedef std::map<std::string, std::map<std::string, std::string>> alias_map_t;
        component_registry()
        {}

        ~component_registry()
        {}

        void register_component(component_registry_entry_base *c)
        {
            std::string component_name(c->name());
            if (contains(m_components, component_name)) {
                MGE_THROW(mge::duplicate_element(),
                          "Try to register duplicate component '",
                          component_name,
                          "'.");
            }
            m_components.insert(std::make_pair(component_name, c));
            update_pending_implementations(component_name);
        }

        void register_implementation(implementation_registry_entry_base *i)
        {
            std::string component_name(i->component_name());
            if (!component_registered(component_name)) {
                m_pending_implementations.emplace_back(i);
            }

            do_register_implementation(component_name, i);
        }

        bool component_registered(const std::string& name) const
        {
            return contains(m_components, name);
        }

        bool implementation_registered(const std::string& component_name,
                                       const std::string& implementation_name) const
        {
            if(m_implementations.find(component_name) == m_implementations.end()) {
                return false;
            }
            const auto& implementations_of_component = m_implementations.find(component_name)->second;
            if (implementations_of_component.find(implementation_name) != implementations_of_component.end()) {
                return true;
            }
            const auto& aliases_of_component = m_aliases.find(component_name)->second;
            if(aliases_of_component.find(implementation_name) != aliases_of_component.end()) {
                return true;
            }
            return false;
        }

        void implementations(const std::string& component_name,
                             const std::function<void (const std::string &)> &callback)
        {
            auto impl_it = m_implementations.find(component_name);
            if(impl_it != m_implementations.end()) {
                for(const auto& e : impl_it->second) {
                    callback(e.first);
                }
            }
        }

        std::shared_ptr<component_base> create(const std::string& component_name,
                                               const char *implementation_name)
        {
            auto impl_it = m_implementations.find(component_name);
            if(impl_it != m_implementations.end()) {
                auto regentry_it = impl_it->second.find(implementation_name);
                if (regentry_it != impl_it->second.end()) {
                    return regentry_it->second->create();
                }
            }

            auto alias_it = m_aliases.find(component_name);
            if(alias_it != m_aliases.end()) {
                auto implname_it = alias_it->second.find(implementation_name);
                if (implname_it != alias_it->second.end()) {
                    return create(component_name, implname_it->second.c_str());
                }
            }

            return std::shared_ptr<component_base>();
        }
    private:
        void update_pending_implementations(const std::string& component_name)
        {
            auto it = m_pending_implementations.begin();
            while (it != m_pending_implementations.end()) {
                if(strcmp((*it)->component_name(), component_name.c_str()) == 0) {
                    auto registry_entry = *it;
                    it = m_pending_implementations.erase(it);
                    do_register_implementation(component_name, registry_entry);
                } else {
                    ++it;
                }
            }
        }

        void do_register_implementation(const std::string& component_name,
                                        implementation_registry_entry_base *i)
        {
            std::string implementation_name(i->name());
            std::string alias_names(i->alias_names());

            m_implementations[component_name][implementation_name] = i;
            std::vector<std::string> alias_name_vec;
            boost::split(alias_name_vec, alias_names, boost::is_any_of(","));
            for (auto& s : alias_name_vec) {
                boost::trim(s);
                m_aliases[component_name][s] = implementation_name;
            }
        }

        component_map_t m_components;
        implementation_map_t m_implementations;
        alias_map_t m_aliases;
        std::vector<implementation_registry_entry_base *> m_pending_implementations;
    };

    singleton<component_registry> s_component_registry;


    void
    component_registry_entry_base::register_component(component_registry_entry_base* c)
    {
        s_component_registry->register_component(c);
    }

    void
    implementation_registry_entry_base::register_implementation(implementation_registry_entry_base *i)
    {
        s_component_registry->register_implementation(i);
    }

    bool
    component_base::component_registered(const std::string &name)
    {
        return s_component_registry->component_registered(name);
    }

    bool
    component_base::implementation_registered(const std::string& component_name,
                                              const std::string& implementation_name)
    {
        return s_component_registry->implementation_registered(component_name,
                                                               implementation_name);
    }

    void
    component_base::implementations(const std::string& component_name,
                                    const std::function<void (const std::string &)> &callback)
    {
        return s_component_registry->implementations(component_name, callback);
    }

    std::shared_ptr<component_base>
    component_base::create(const std::string& component_name,
                           const char *implementation_name)
    {
        return s_component_registry->create(component_name, implementation_name);
    }

}
