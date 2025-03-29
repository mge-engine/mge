// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/component.hpp"
#include "boost/boost_algorithm_string.hpp"
#include "mge/core/contains.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"
#include "mge/core/type_name.hpp"
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace mge {

    MGE_USE_TRACE(CORE);

    class default_component_registry
    {
    public:
        using component_map =
            std::map<std::string_view, component_registry_entry_base*>;
        using implementation_map = std::map<
            std::string_view,
            std::map<std::string_view, implementation_registry_entry_base*>>;
        using alias_map =
            std::map<std::string_view, std::map<std::string, std::string_view>>;

        default_component_registry() = default;

        ~default_component_registry() = default;

        void register_component(component_registry_entry_base* c)
        {
            auto component_name = c->name();
            MGE_TRACE(CORE, DEBUG)
                << "Registering component " << component_name;
            if (contains(m_components, component_name)) {
                MGE_THROW(mge::duplicate_element)
                    << "Try to register duplicate component '" << component_name
                    << "'";
            }
            m_components.insert(std::make_pair(component_name, c));
            update_pending_implementations(component_name);
        }

        void register_implementation(implementation_registry_entry_base* i)
        {
            auto component_name = i->component_name();
            MGE_TRACE(CORE, DEBUG) << "Registering implementation " << i->name()
                                   << " of component " << component_name;
            if (!component_registered(component_name)) {
                m_pending_implementations.emplace_back(i);
            }

            do_register_implementation(component_name, i);
        }

        bool component_registered(std::string_view name) const
        {
            return contains(m_components, name);
        }

        bool
        implementation_registered(std::string_view component_name,
                                  std::string_view implementation_name) const
        {
            if (m_implementations.find(component_name) ==
                m_implementations.end()) {
                return false;
            }
            const auto& implementations_of_component =
                m_implementations.find(component_name)->second;
            if (implementations_of_component.find(implementation_name) !=
                implementations_of_component.end()) {
                return true;
            }
            {
                std::string alias_implementation_name(implementation_name);
                const auto& aliases_of_component =
                    m_aliases.find(component_name)->second;

                if (aliases_of_component.find(alias_implementation_name) !=
                    aliases_of_component.end()) {
                    return true;
                }
            }
            return false;
        }

        void
        implementations(std::string_view component_name,
                        const std::function<void(std::string_view)>& callback)
        {
            auto impl_it = m_implementations.find(component_name);
            if (impl_it != m_implementations.end()) {
                for (const auto& e : impl_it->second) {
                    callback(e.first);
                }
            }
        }

        std::shared_ptr<component_base>
        create(std::string_view component_name,
               std::string_view implementation_name)
        {
            MGE_TRACE(CORE, DEBUG)
                << "Create instance of " << component_name
                << " using implementation " << implementation_name;

            auto impl_it = m_implementations.find(component_name);
            if (impl_it != m_implementations.end()) {
                auto regentry_it = impl_it->second.find(implementation_name);
                if (regentry_it != impl_it->second.end()) {
                    auto result = regentry_it->second->create();
                    result->m_impl_regentry = regentry_it->second;
                    return result;
                }
            }

            auto alias_it = m_aliases.find(component_name);
            if (alias_it != m_aliases.end()) {
                std::string alias_implementation_name(implementation_name);
                auto        implname_it =
                    alias_it->second.find(alias_implementation_name);
                if (implname_it != alias_it->second.end()) {
                    return create(component_name, implname_it->second);
                }
            }

            return std::shared_ptr<component_base>();
        }

    private:
        void update_pending_implementations(std::string_view component_name)
        {
            auto it = m_pending_implementations.begin();
            while (it != m_pending_implementations.end()) {
                if ((*it)->component_name() == component_name) {
                    auto registry_entry = *it;
                    it = m_pending_implementations.erase(it);
                    do_register_implementation(component_name, registry_entry);
                } else {
                    ++it;
                }
            }
        }

        void do_register_implementation(std::string_view component_name,
                                        implementation_registry_entry_base* i)
        {
            auto implementation_name = i->name();

            std::string alias_names(i->alias_names());

            m_implementations[component_name][implementation_name] = i;
            std::vector<std::string> alias_name_vec;
            boost::split(alias_name_vec, alias_names, boost::is_any_of(","));
            for (auto& s : alias_name_vec) {
                boost::trim(s);
                m_aliases[component_name][s] = implementation_name;
            }
        }

        component_map      m_components;
        implementation_map m_implementations;
        alias_map          m_aliases;
        std::vector<implementation_registry_entry_base*>
            m_pending_implementations;
    };

    singleton<default_component_registry> s_component_registry;

    void component_registry_entry_base::register_component(
        component_registry_entry_base* c)
    {
        s_component_registry->register_component(c);
    }

    void implementation_registry_entry_base::register_implementation(
        implementation_registry_entry_base* i)
    {
        s_component_registry->register_implementation(i);
    }

    bool component_base::component_registered(std::string_view name)
    {
        return s_component_registry->component_registered(name);
    }

    bool component_base::implementation_registered(
        std::string_view component_name, std::string_view implementation_name)
    {
        return s_component_registry->implementation_registered(
            component_name,
            implementation_name);
    }

    void component_base::implementations(
        std::string_view                             component_name,
        const std::function<void(std::string_view)>& callback)
    {
        return s_component_registry->implementations(component_name, callback);
    }

    std::shared_ptr<component_base>
    component_base::create(std::string_view component_name,
                           std::string_view implementation_name)
    {
        return s_component_registry->create(component_name,
                                            implementation_name);
    }

    component_registry::component_registry() {}

    component_registry::~component_registry() {}
} // namespace mge
