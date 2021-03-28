// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/configuration.hpp"
#include "mge/core/executable_name.hpp"
#include "mge/core/parameter.hpp"
#include "mge/core/singleton.hpp"

// property tree use deprecated boost bind
// placeholders
#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <filesystem>
#include <map>
#include <stdexcept>

namespace fs = std::filesystem;
namespace pt = boost::property_tree;

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

        void load();
        void store();

    private:
        fs::path find_config_file();
        void     set_registered_parameters();
        void     fetch_parameter(basic_parameter &p);
        void     write_parameter(basic_parameter &p);

        using parameter_map = std::map<std::string_view, basic_parameter *>;
        using section_map   = std::map<std::string_view, parameter_map>;

        section_map m_sections;
        pt::ptree   m_raw_settings;
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
        if (!m_raw_settings.empty()) {
            fetch_parameter(p);
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

    fs::path configuration_instance::find_config_file()
    {
        const char *suffixes[] = {"json", "ini", "xml", "info", 0};
        auto        base_name  = executable_name();

        if (fs::is_directory("config")) {
            fs::path dir("config");

            for (const char **suffix = suffixes; *suffix; ++suffix) {
                std::string fname(base_name);
                fname += ".";
                fname += *suffix;
                fs::path fullpath = dir / fname;
                if (fs::exists(fullpath) && fs::is_regular_file(fullpath)) {
                    return fullpath;
                }
            }
        }
        for (const char **suffix = suffixes; *suffix; ++suffix) {
            std::string fname(base_name);
            fname += ".";
            fname += *suffix;
            fs::path fullpath(fname);
            if (fs::exists(fullpath) && fs::is_regular_file(fullpath)) {
                return fullpath;
            }
        }

        return fs::path();
    }

    void configuration_instance::load()
    {
        auto configfile_path = find_config_file();
        if (configfile_path.empty()) {
            return;
        }
        std::ifstream input(configfile_path);
        if (configfile_path.extension() == ".xml") {
            boost::property_tree::read_xml(input, m_raw_settings);
        } else if (configfile_path.extension() == ".json") {
            boost::property_tree::read_json(input, m_raw_settings);
        } else if (configfile_path.extension() == ".ini") {
            boost::property_tree::read_ini(input, m_raw_settings);
        } else if (configfile_path.extension() == ".info") {
            boost::property_tree::read_info(input, m_raw_settings);
        }
        set_registered_parameters();
    }

    void configuration_instance::set_registered_parameters()
    {
        for (auto &s : m_sections) {
            for (auto &p : s.second) {
                fetch_parameter(*p.second);
            }
        }
    }

    void configuration_instance::fetch_parameter(basic_parameter &p)
    {
        pt::ptree::path_type parameter_path;
        parameter_path /= std::string(p.section().begin(), p.section().end());
        parameter_path /= std::string(p.name().begin(), p.name().end());

        auto ov = m_raw_settings.get_optional<std::string>(parameter_path);
        if (ov.has_value()) {
            p.from_string(ov.get());
        }
    }

    void configuration_instance::store()
    {
        for (auto &s : m_sections) {
            for (auto &p : s.second) {
                write_parameter(*p.second);
            }
        }

        auto file_path = find_config_file();
        if (file_path.empty()) {
            std::string name = executable_name();
            name += ".json";
            file_path = fs::path(name);
        }
        std::ofstream output(file_path);

        if (file_path.extension() == ".json") {
            boost::property_tree::write_json(output, m_raw_settings);
        } else if (file_path.extension() == ".xml") {
            boost::property_tree::write_xml(output, m_raw_settings);
        } else if (file_path.extension() == ".info") {
            boost::property_tree::write_info(output, m_raw_settings);
        } else if (file_path.extension() == ".ini") {
            boost::property_tree::write_ini(output, m_raw_settings);
        }
    }

    void configuration_instance::write_parameter(basic_parameter &p)
    {
        pt::ptree::path_type parameter_path;
        parameter_path /= std::string(p.section().begin(), p.section().end());
        parameter_path /= std::string(p.name().begin(), p.name().end());
        pt::ptree ::assoc_iterator x;

        if (p.has_value()) {
            m_raw_settings.put(parameter_path, p.to_string());
        } else {
            auto section_it = m_raw_settings.find(
                std::string(p.section().begin(), p.section().end()));
            if (section_it != m_raw_settings.not_found()) {
                auto &value_tree = section_it->second;
                auto  value_it   = value_tree.find(
                    std::string(p.name().begin(), p.name().end()));
                if (value_it != value_tree.not_found()) {
                    value_tree.erase(value_tree.to_iterator(value_it));
                }
            }
        }
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

    void configuration::load() { s_configuration_instance->load(); }

    void configuration::store() { s_configuration_instance->store(); }

} // namespace mge