// mge - Modern Game Engine
// Copyright (c) 2019 by Alexander Schroeder
// All rights reserved.
#include "mge/core/configuration.hpp"
#include "mge/core/parameter.hpp"
#include "mge/core/singleton.hpp"
#include <map>
#include <stdexcept>
namespace mge {
    class configuration_instance
    {
    public:
        configuration_instance()  = default;
        ~configuration_instance() = default;

        void register_parameter(basic_parameter &p);
        void unregister_parameter(basic_parameter &p);

        basic_parameter &find_parameter(std::string_view section,
                                        std::string_view name);

    private:
        using parameter_map = std::map<std::string_view, basic_parameter *>;
        using section_map   = std::map<std::string_view, parameter_map>;

        section_map m_sections;
    };

    void configuration_instance::register_parameter(basic_parameter &p)
    {
        auto pmap_it = m_sections.find(p.section());
        if (pmap_it != m_sections.end()) {
            auto p_it = pmap_it->second.find(p.name());
            if (p_it != pmap_it->second.end()) {
                throw std::runtime_error("Duplicate parameter");
            } else {
                pmap_it->second[p.name()] = &p;
            }
        } else {
            m_sections[p.section()][p.name()] = &p;
        }
    }

    void configuration_instance::unregister_parameter(basic_parameter &p)
    {
        auto pmap_it = m_sections.find(p.section());
        if (pmap_it != m_sections.end()) {
            auto p_it = pmap_it->second.find(p.name());
            if (p_it != pmap_it->second.end()) {
                pmap_it->second.erase(p_it);
            }
        }
    }

    basic_parameter &
    configuration_instance::find_parameter(std::string_view section,
                                           std::string_view name)
    {
        auto pmap_it = m_sections.find(section);
        if (pmap_it != m_sections.end()) {
            auto p_it = pmap_it->second.find(name);
            if (p_it != pmap_it->second.end()) {
                return *p_it->second;
            }
        }
        throw std::runtime_error("Unknown parameter");
    }

    static singleton<configuration_instance> s_configuration_instance;

    void configuration::register_parameter(basic_parameter &p)
    {
        s_configuration_instance->register_parameter(p);
    }

    void configuration::unregister_parameter(basic_parameter &p)
    {
        s_configuration_instance->unregister_parameter(p);
    }

    basic_parameter &configuration::find_parameter(std::string_view section,
                                                   std::string_view name)
    {
        return s_configuration_instance->find_parameter(section, name);
    }

} // namespace mge