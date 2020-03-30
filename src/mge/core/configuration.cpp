// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/configuration.hpp"
#include "mge/core/configuration_database.hpp"

#include <boost/algorithm/string.hpp>

#include <iostream>

namespace mge {
    static configuration::config_values_map &configuration_overrides()
    {
        static configuration::config_values_map overrides;
        return overrides;
    }

    configuration::configuration(const configuration::transient_tag &)
        : m_transient(true)
    {}

    configuration::configuration() : m_transient(false)
    {
        configuration_database::instance().read_configuration_values(m_values);
    }

    configuration::configuration(const std::string &prefix)
        : m_prefix(prefix), m_transient(false)
    {
        configuration_database::instance().read_configuration_values(
            prefix.c_str(), m_values);
    }

    configuration::configuration(std::string &&prefix)
        : m_prefix(std::move(prefix)), m_transient(false)
    {
        configuration_database::instance().read_configuration_values(
            m_prefix.c_str(), m_values);
    }

    configuration::configuration(const configuration &c)
        : m_prefix(c.m_prefix), m_values(c.m_values), m_transient(c.m_transient)
    {}

    configuration::configuration(configuration &&c)
        : m_prefix(std::move(c.m_prefix)), m_values(std::move(c.m_values)),
          m_transient(false)
    {}

    configuration &configuration::operator=(const configuration &c)
    {
        m_prefix    = c.m_prefix;
        m_values    = c.m_values;
        m_transient = c.m_transient;
        return *this;
    }

    void configuration::store()
    {
        if (m_transient) {
            MGE_THROW(mge::illegal_state)
                << "Cannot store transient configuration";
        }

        if (m_prefix.empty()) {
            configuration_database::instance().store_configuration_values(
                m_values);
        } else {
            configuration_database::instance().store_configuration_values(
                m_prefix.c_str(), m_values);
        }
    }

    void configuration::clear()
    {
        configuration_database::instance().clear_configuration(
            m_prefix.c_str());
        m_values.clear();
    }

    const std::string &configuration::value(const std::string &key) const
    {
        if (m_prefix.empty()) {
            auto override_i = configuration_overrides().find(key);
            if (override_i != configuration_overrides().end()) {
                return override_i->second;
            }
        } else {
            auto override_i =
                configuration_overrides().find(m_prefix + "." + key);
            if (override_i != configuration_overrides().end()) {
                return override_i->second;
            }
        }

        auto i = m_values.find(key);
        if (i == m_values.end()) {
            MGE_THROW(configuration_key_not_found)
                << "Configuration key '" << key << "' not found";
        } else {
            return i->second;
        }
    }

    std::vector<std::string>
    configuration::list_value(const std::string &key) const
    {
        std::string              liststring = value(key);
        std::vector<std::string> result;
        boost::split(result, liststring, boost::is_any_of(","));
        for (auto &s : result) {
            boost::trim(s);
        }
        return result;
    }

    bool configuration::contains_key(const char *key) const
    {
        return contains_key(std::string(key));
    }

    bool configuration::contains_key(const std::string &key) const
    {
        if (m_prefix.empty()) {
            auto override_i = configuration_overrides().find(key);
            if (override_i != configuration_overrides().end()) {
                return true;
            }
        } else {
            auto override_i =
                configuration_overrides().find(m_prefix + "." + key);
            if (override_i != configuration_overrides().end()) {
                return true;
            }
        }
        auto i = m_values.find(key);
        return i != m_values.end();
    }

    void configuration::set_override(const char *key, const char *value)
    {
        configuration_overrides()[key] = value;
    }

    void configuration::set_override(const std::string &key,
                                     const std::string &value)
    {
        configuration_overrides()[key] = value;
    }

    void configuration::clear_override(const char *key)
    {
        auto i = configuration_overrides().find(key);
        if (i != configuration_overrides().end()) {
            configuration_overrides().erase(i);
        }
    }

    void configuration::clear_override(const std::string &key)
    {
        auto i = configuration_overrides().find(key);
        if (i != configuration_overrides().end()) {
            configuration_overrides().erase(i);
        }
    }

    configuration_key_not_found::configuration_key_not_found() {}

    configuration_key_not_found::configuration_key_not_found(
        const configuration_key_not_found &e)
        : no_such_element(e)
    {}

    configuration_key_not_found::configuration_key_not_found(
        configuration_key_not_found &&e)
        : no_such_element(std::move(e))
    {}

    configuration_key_not_found &
    configuration_key_not_found::operator=(const configuration_key_not_found &e)
    {
        no_such_element::operator=(e);
        return *this;
    }

    configuration_key_not_found::~configuration_key_not_found() {}

} // namespace mge
