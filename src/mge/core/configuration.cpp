// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/configuration.hpp"
#include "mge/core/executable_name.hpp"
#include "mge/core/parameter.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"

// property tree use deprecated boost bind
// placeholders
#include "boost/boost_property_tree.hpp"

#include <filesystem>
#include <map>
#include <stdexcept>

namespace fs = std::filesystem;
namespace pt = boost::property_tree;

namespace mge {

    MGE_USE_TRACE(CORE);

    class configuration_instance
    {
    public:
        configuration_instance()
            : m_loaded(false)
            , m_update_needed(true)
        {}

        ~configuration_instance() = default;

        void register_parameter(basic_parameter& p);
        void unregister_parameter(basic_parameter& p);

        basic_parameter& find_parameter(std::string_view section,
                                        std::string_view name);

        std::optional<std::reference_wrapper<basic_parameter>>
             find_optional_parameter(std::string_view section,
                                     std::string_view name);
        void load(bool allow_missing);
        void store();
        bool loaded() const { return m_loaded; }
        void set_raw(std::string_view section,
                     std::string_view name,
                     std::string_view value);

    private:
        fs::path find_config_file();
        void     set_registered_parameters();
        void     fetch_parameter(basic_parameter& p);
        void     write_parameter(basic_parameter& p);

        using parameter_map = std::map<mge::path, basic_parameter*>;

        parameter_map m_parameters;
        pt::ptree     m_raw_settings;
        bool          m_loaded;
        bool          m_update_needed;
    };

    void configuration_instance::register_parameter(basic_parameter& p)
    {
        MGE_DEBUG_TRACE(CORE) << "Register parameter " << p.path();
        m_parameters[p.path()] = &p;
        m_update_needed = true;
    }

    void configuration_instance::unregister_parameter(basic_parameter& p)
    {
        auto it = m_parameters.find(p.path());
        if (it != m_parameters.end()) {
            m_parameters.erase(it);
        }
    }

    std::optional<std::reference_wrapper<basic_parameter>>
    configuration_instance::find_optional_parameter(std::string_view section,
                                                    std::string_view name)
    {
        std::optional<std::reference_wrapper<basic_parameter>> result;

        mge::path key;
        key /= section;
        key /= name;

        if (m_update_needed && m_loaded) {
            set_registered_parameters();
            m_update_needed = false;
        }
        auto p_it = m_parameters.find(key);
        if (p_it != m_parameters.end()) {
            return *p_it->second;
        }
        return result;
    }

    basic_parameter&
    configuration_instance::find_parameter(std::string_view section,
                                           std::string_view name)
    {
        if (m_update_needed && m_loaded) {
            set_registered_parameters();
            m_update_needed = false;
        }

        mge::path key;
        key /= section;
        key /= name;

        auto p_it = m_parameters.find(key);
        if (p_it != m_parameters.end()) {
            return *p_it->second;
        }
        MGE_THROW(mge::runtime_exception)
            << "Unknown parameter " << section << "/" << name;
    }

    fs::path configuration_instance::find_config_file()
    {
        const char* suffixes[] = {"json", "ini", "xml", "info", 0};
        auto        base_name = executable_name();

        if (fs::is_directory("config")) {
            fs::path dir("config");

            for (const char** suffix = suffixes; *suffix; ++suffix) {
                std::string fname(base_name);
                fname += ".";
                fname += *suffix;
                fs::path fullpath = dir / fname;
                if (fs::exists(fullpath) && fs::is_regular_file(fullpath)) {
                    return fullpath;
                }
            }
        }
        for (const char** suffix = suffixes; *suffix; ++suffix) {
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

    void configuration_instance::load(bool allow_missing)
    {
        auto configfile_path = find_config_file();
        if (configfile_path.empty() && !allow_missing) {
            return;
        } else {
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
        }
        set_registered_parameters();
        m_loaded = true;
    }

    void configuration_instance::set_registered_parameters()
    {
        for (auto& p : m_parameters) {
            fetch_parameter(*p.second);
        }
    }

    void configuration_instance::set_raw(std::string_view section,
                                         std::string_view name,
                                         std::string_view value)
    {
        MGE_DEBUG_TRACE(CORE) << "Set parameter value " << section << "/"
                              << name << " to '" << value << "'";

        pt::ptree::path_type parameter_path;
        parameter_path /= std::string(section.begin(), section.end());
        parameter_path /= std::string(name.begin(), name.end());

        m_raw_settings.put(parameter_path, value);
        auto param = find_optional_parameter(section, name);
        if (param.has_value()) {
            param->get().from_string(value);
            param->get().notify_change();
        }
    }

    void configuration_instance::fetch_parameter(basic_parameter& p)
    {
        pt::ptree::path_type parameter_path;
        for (const auto& e : p.path()) {
            parameter_path /= e.string();
        }
        if (p.value_type() == basic_parameter::type::VALUE) {
            auto ov = m_raw_settings.get_optional<std::string>(parameter_path);
            if (ov.has_value()) {
                MGE_DEBUG_TRACE(CORE)
                    << "Set parameter " << p.path() << " to " << ov.get();
                p.from_string(ov.get());
            } else {
                MGE_DEBUG_TRACE(CORE) << "Reset parameter " << p.path();
                p.reset();
            }
            MGE_DEBUG_TRACE(CORE) << "Notify change of parameter " << p.path();
            p.notify_change();
        } else {
        }
    }

    void configuration_instance::store()
    {
        for (auto& p : m_parameters) {
            write_parameter(*p.second);
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

    void configuration_instance::write_parameter(basic_parameter& p)
    {
        if (p.has_value()) {
            if (p.value_type() == basic_parameter::type::VALUE) {
                pt::ptree::path_type parameter_path;
                for (const auto& e : p.path()) {
                    parameter_path /= e.string();
                }
                if (p.has_value()) {
                    m_raw_settings.put(parameter_path, p.to_string());
                }
            } else {
            }
        } else {
            std::string folder_path;
            auto        i = p.path().begin();
            auto        e = p.path().end();
            if (i == e) {
                MGE_THROW(illegal_state)
                    << "Parameter path must contain 2 or more components";
            }
            folder_path = i->string();
            ++i;
            if (i == e) {
                MGE_THROW(illegal_state)
                    << "Parameter path must contain 2 or more components";
            }
            --e;
            while (i != e) {
                folder_path += ".";
                folder_path += i->string();
                ++i;
            }
            auto folder_it = m_raw_settings.find(folder_path);
            if (folder_it != m_raw_settings.not_found()) {
                auto& value_tree = folder_it->second;
                auto  value_it = value_tree.find(e->string());
                if (value_it != value_tree.not_found()) {
                    value_tree.erase(value_tree.to_iterator(value_it));
                }
            }
        }
    }

    static singleton<configuration_instance> s_configuration_instance;

    void configuration::register_parameter(basic_parameter& p)
    {
        s_configuration_instance->register_parameter(p);
    }

    void configuration::unregister_parameter(basic_parameter& p)
    {
        s_configuration_instance->unregister_parameter(p);
    }

    basic_parameter& configuration::find_parameter(std::string_view section,
                                                   std::string_view name)
    {
        return s_configuration_instance->find_parameter(section, name);
    }

    void configuration::load(bool allow_missing)
    {
        s_configuration_instance->load(allow_missing);
    }

    void configuration::evaluate_command_line(
        const std::vector<const char*>& cmdline)
    {
        using namespace std::literals;

        if (cmdline.size() < 1u) {
            return;
        }

        auto it = cmdline.begin();
        ++it;
        while (it != cmdline.end()) {
            if (*it == "--config"sv) {
                ++it;
                if (it != cmdline.end()) {
                    std::string_view cfgval = *it;

                    auto pos = cfgval.find_first_of("=");

                    if (pos != std::string_view::npos) {
                        std::string_view key(cfgval.begin(),
                                             cfgval.begin() + pos);
                        std::string_view val;
                        if (pos + 1 < cfgval.size()) {
                            val = std::string_view(cfgval.begin() + pos + 1,
                                                   cfgval.end());
                        }

                        auto sectionpos = key.find_first_of(".");
                        if (sectionpos != std::string_view::npos &&
                            sectionpos + 1 < key.size()) {
                            std::string_view section(key.begin(),
                                                     key.begin() + sectionpos);
                            std::string_view name(key.begin() + sectionpos + 1,
                                                  key.end());
                            s_configuration_instance->set_raw(section,
                                                              name,
                                                              val);
                        }
                    }
                    ++it;
                } else {
                    ++it;
                }
            } else {
                ++it;
            }
        }
    }

    void configuration::store() { s_configuration_instance->store(); }

    bool configuration::loaded() { return s_configuration_instance->loaded(); }

} // namespace mge