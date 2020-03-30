// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once

#include <map>
#include <string>
#include <vector>

#include "mge/core/bool_value.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/lexical_cast.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {

    /**
     * Exception to signal a configuration key was not found.
     */
    class MGE_CORE_EXPORT configuration_key_not_found : public no_such_element
    {
    public:
        configuration_key_not_found();
        configuration_key_not_found(const configuration_key_not_found &e);
        configuration_key_not_found(configuration_key_not_found &&e);
        virtual ~configuration_key_not_found();
        configuration_key_not_found &
        operator=(const configuration_key_not_found &e);

        template <typename Info>
        configuration_key_not_found &set_info(const Info &info)
        {
            mge::exception::set_info(info);
            return *this;
        }

        template <typename T>
        configuration_key_not_found &operator<<(const T &value)
        {
            mge::exception::operator<<(value);
            return *this;
        }
    };

    /**
     * Configuration access.
     */
    class MGE_CORE_EXPORT configuration
    {
    public:
        struct transient_tag
        {};

        /**
         * @brief Object to signal a configuration is transient.
         */
        static constexpr transient_tag transient = {};

        /**
         * Map of configuration values.
         */
        using config_values_map = std::map<std::string, std::string>;

        /**
         * List of configuration keys.
         */
        using key_list = std::vector<std::string>;

        /**
         * Create a transient configuration. A transient configuration
         * cannot be saved into the applications's configuration.
         */
        configuration(const configuration::transient_tag &);

        /**
         * Default constructor, creates empty configuration.
         */
        configuration();

        /**
         * Constructor.
         * @param prefix configuration prefix (or section)
         */
        explicit configuration(const std::string &prefix);

        /**
         * Constructor (move variant).
         * @param prefix configuration prefix (or section)
         */
        explicit configuration(std::string &&prefix);

        /**
         * Copy constructor.
         * @param c copied configuration
         */
        configuration(const configuration &c);

        /**
         * Copy constructor (move variant).
         * @param c copied configuration
         */
        configuration(configuration &&c);

        /**
         * Assignment operator.
         * @param c assigned configuration
         * @return @c *this
         */
        configuration &operator=(const configuration &c);

        /**
         * Return whether configuration value exist for a key.
         * @param key configuration key
         * @return @c true if value exists
         */
        bool contains_key(const char *key) const;

        /**
         * Return whether configuration value exist for a key.
         * @param key configuration key
         * @return @c true if value exists
         */
        bool contains_key(const std::string &key) const;

        /**
         * Get configuration keys.
         * @return list of configuration keys
         */
        key_list keys() const
        {
            key_list result;
            for (auto p : m_values) {
                result.push_back(p.first);
            }
            return result;
        }

        /**
         * Get configuration value.
         * @param key configuration key
         * @return configuration value
         * @throw A @c configuration_key_not_found exception is thrown if the
         *        key is not found.
         */
        std::string value(const char *key) const
        {
            std::string keystr(key);
            return value(keystr);
        }

        /**
         * Treats the configuration value as a list, with values
         * simply separated by ','.
         * @param key configuration key
         * @return vector of strings
         * @throw A @c configuration_key_not_found exception is thrown if the
         *        key is not found.
         */
        inline std::vector<std::string> list_value(const char *key) const
        {
            return list_value(std::string(key));
        }

        /**
         * Treats the configuration value as a list, with values
         * simply separated by ','.
         * @param key configuration key
         * @return vector of strings
         * @throw A @c configuration_key_not_found exception is thrown if the
         *        key is not found.
         */
        std::vector<std::string> list_value(const std::string &key) const;

        /**
         * Get configuration value.
         * @param key configuration key
         * @return configuration value
         * @throw A @c configuration_key_not_found exception is thrown if the
         *        key is not found.
         */
        const std::string &value(const std::string &key) const;

        /**
         * Get configuration value.
         * @param key configuration key
         * @param default_value value used if key not found
         * @return configuration value, @c default_value if no such entry
         *         is found
         */
        inline std::string value(const char *       key,
                                 const std::string &default_value) const
        {
            try {
                return value(key);
            } catch (const configuration_key_not_found &) {
                return default_value;
            }
        }

        /**
         * Get configuration value.
         * @param key configuration key
         * @param default_value value used if key not found
         * @return configuration value, @c default_value if no such entry
         *         is found
         */
        template <typename T>
        inline T value(const char *key, const T &default_value) const
        {
            try {
                return lexical_cast<T>(value(key));
            } catch (const configuration_key_not_found &) {
                return default_value;
            }
        }

        /**
         * Get configuration value.
         * @param key configuration key
         * @return configuration value
         */
        template <typename T> inline T value(const char *key) const
        {
            return lexical_cast<T>(value(key));
        }

        /**
         * Get configuration value.
         * @param key configuration key
         * @param default_value value used if key not found
         * @return configuration value, @c default_value if no such entry
         *         is found
         */
        template <typename T>
        inline T value(const std::string &key, const T &default_value) const
        {
            try {
                return lexical_cast<T>(value(key));
            } catch (const configuration_key_not_found &) {
                return default_value;
            }
        }

        /**
         * Get configuration value.
         * @param key configuration key
         * @return configuration value
         */
        template <typename T> inline T value(const std::string &key) const
        {
            return lexical_cast<T>(value(key));
        }

        /**
         * Return whether configuration has no entries.
         * @return @c true if configuration has no entries
         */
        inline bool empty() const { return m_values.empty(); }

        /**
         * Set a configuration value.
         * @param key configuration key
         * @param value configuration value
         */
        template <typename T> inline void set(const char *key, const T &value)
        {
            m_values[key] = lexical_cast<std::string>(value);
        }

        /**
         * Store the configuration persistently.
         */
        void store();

        /**
         * Clear the configuration.
         */
        void clear();

        /**
         * Set a transient override for a stored configuration value.
         * @param key   configuration key
         * @param value configuration value
         */
        static void set_override(const char *key, const char *value);

        /**
         * Set a transient override for a stored configuration value.
         * @param key   configuration key
         * @param value configuration value
         */
        static void set_override(const std::string &key,
                                 const std::string &value);

        /**
         * Deletes transient override value.
         * @param key configuration key
         */
        static void clear_override(const char *key);

        /**
         * Deletes transient override value.
         * @param key configuration key
         */
        static void clear_override(const std::string &key);

    private:
        std::string       m_prefix;
        config_values_map m_values;
        bool              m_transient;
    };

    template <> inline bool configuration::value<bool>(const char *key) const
    {
        return bool_value(value(key));
    }

    template <>
    inline bool configuration::value<bool>(const char *key,
                                           const bool &default_value) const
    {
        try {
            return bool_value(value(key));
        } catch (const configuration_key_not_found &) {
            return default_value;
        }
    }

    template <>
    inline bool configuration::value<bool>(const std::string &key) const
    {
        return bool_value(value(key));
    }

    template <>
    inline bool configuration::value<bool>(const std::string &key,
                                           const bool &default_value) const
    {
        try {
            return bool_value(value(key));
        } catch (const configuration_key_not_found &) {
            return default_value;
        }
    }
} // namespace mge
