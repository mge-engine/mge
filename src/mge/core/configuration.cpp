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
#include "nlohmann/json.hpp"
#include "simdjson.h"

#include <filesystem>
#include <map>
#include <stdexcept>

namespace fs = std::filesystem;

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

        basic_parameter& find_parameter(const mge::path& path);
        basic_parameter& find_parameter(std::string_view section,
                                        std::string_view name);

        std::optional<std::reference_wrapper<basic_parameter>>
             find_optional_parameter(std::string_view section,
                                     std::string_view name);
        void load();
        void store();
        bool loaded() const { return m_loaded; }
        void set_raw(std::string_view section,
                     std::string_view name,
                     std::string_view value);

        const mge::json::json& root() { return m_raw_settings; }

    private:
        fs::path find_config_file();
        void     set_registered_parameters();
        void     fetch_parameter(basic_parameter& p);
        void     write_parameter(basic_parameter& p);
        void     erase_parameter(basic_parameter& p);

        using parameter_map = std::map<mge::path, basic_parameter*>;

        parameter_map m_parameters;
        json::json    m_raw_settings;
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
    configuration_instance::find_parameter(const mge::path& path)
    {
        if (m_update_needed && m_loaded) {
            set_registered_parameters();
            m_update_needed = false;
        }

        auto p_it = m_parameters.find(path);
        if (p_it != m_parameters.end()) {
            return *p_it->second;
        }
        MGE_THROW(mge::runtime_exception) << "Unknown parameter " << path;
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
        const char* suffixes[] = {"json", 0};
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

    void configuration_instance::load()
    {
        auto configfile_path = find_config_file();
        if (configfile_path.empty()) {
            return;
        }

        std::ifstream ifs(configfile_path);
        try {
            m_raw_settings = json::json::parse(ifs);
        } catch (json::json::parse_error& e) {
            MGE_THROW(mge::runtime_exception)
                << "Error parsing configuration file " << configfile_path
                << ": " << e.what();
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

        mge::json::json_pointer<mge::json::json::string_t> parameter_path;
        parameter_path /= std::string(section.begin(), section.end());
        parameter_path /= std::string(name.begin(), name.end());

        m_raw_settings[parameter_path] = value;
        auto param = find_optional_parameter(section, name);
        if (param.has_value()) {
            MGE_THROW_NOT_IMPLEMENTED;
            param->get().notify_change();
        }
    }

    void configuration_instance::fetch_parameter(basic_parameter& p)
    {
        p.read_value(m_raw_settings);
        p.notify_change();
    }

    void configuration_instance::store()
    {
        for (auto& p : m_parameters) {
            write_parameter(*p.second);
        }

        std::string name = executable_name();
        name += ".json";
        fs::path file_path = fs::path(name);
    }

    void configuration_instance::erase_parameter(basic_parameter& p)
    {
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
        if (folder_it != m_raw_settings.end()) {
            m_raw_settings.erase(folder_it);
        }
    }

    void configuration_instance::write_parameter(basic_parameter& p)
    {
        if (p.has_value()) {
            p.write_value(m_raw_settings);
        } else {
            erase_parameter(p);
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

    void configuration::load() { s_configuration_instance->load(); }

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

    const mge::json::json& configuration::root()
    {
        return s_configuration_instance->root();
    }

} // namespace mge