// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/component.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/contains.hpp"
#include "mge/core/stdexceptions.hpp"

#include <unordered_map>
#include <string>

namespace mge {

    class component_registry
    {
    public:
        typedef std::unordered_map<std::string, component_registry_entry_base *> component_map_t;
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
        }

        void register_implementation(implementation_registry_entry_base *i)
        {
            return;
        }

        component_map_t m_components;
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
}
