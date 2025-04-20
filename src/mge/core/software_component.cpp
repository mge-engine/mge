// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/software_component.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/version.hpp"

#include <set>
#include <string_view>

using namespace std::literals::string_view_literals;

namespace mge {
    MGE_REGISTER_COMPONENT(software_component);

    MGE_DEFINE_SOFTWARE_COMPONENT(mge,
                                  semantic_version(MGE_VERSION_MAJOR,
                                                   MGE_VERSION_MINOR,
                                                   MGE_PATCHLEVEL),
                                  MGE_BUILD);

    class software_component_list
    {
    public:
        software_component_list() = default;
        ~software_component_list() = default;

        software_component_ref get(std::string_view name,
                                   bool throw_no_such_element = false)
        {
            auto it = m_components.find(name);
            if (it != m_components.end()) {
                return it->second;
            }
            software_component::implementations(
                [&](std::string_view impl_name) {
                    auto impl_it =
                        m_software_component_implementations.find(impl_name);
                    if (impl_it == m_software_component_implementations.end()) {
                        m_software_component_implementations.insert(
                            std::string(impl_name.begin(), impl_name.end()));
                        auto component = software_component::create(impl_name);
                        m_components.insert(
                            std::make_pair(component->name(), component));
                    }
                });
            it = m_components.find(name);
            if (it != m_components.end()) {
                return it->second;
            }
            if (throw_no_such_element) {
                MGE_THROW(mge::no_such_element)
                    << "Software component '" << name << "' not found";
            }
            return nullptr;
        }

        std::map<std::string, software_component_ref, std::less<>> m_components;
        std::set<std::string, std::less<>> m_software_component_implementations;
    };

    static singleton<software_component_list> s_software_component_list;

    software_component_ref software_component::mge()
    {
        return s_software_component_list->get(std::string_view("mge"), true);
    }
} // namespace mge