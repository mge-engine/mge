// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/input_stream.hpp"
#include "mge/core/lexical_cast.hpp"
#include "mge/core/memory.hpp"
#include "mge/core/stdexceptions.hpp"

#include <string_view>
#include <unordered_map>

namespace mge {

    MGE_DECLARE_REF(properties);

    /**
     * Properties represent a map of key-value pairs.
     * Both key and value are strings, but utility
     * methods to cast values into other types exist.
     */
    class MGECORE_EXPORT properties
    {
    public:
        using map_type = std::map<std::string, std::string, std::less<>>;
        using const_iterator = map_type::const_iterator;
        using iterator = map_type::iterator;
        using value_type = map_type::value_type;

        /**
         * @brief Construct empty properties.
         */
        properties();
        /**
         * @brief Copy constructor.
         * @param p properties to copy
         */
        properties(const properties& p);
        /**
         * @brief Move constructor.
         * @param p properties to move
         */
        properties(properties&& p);
        /**
         * @brief Load properties from input stream.
         * @param input input stream
         */
        properties(std::istream& input);
        /**
         * @brief Load properties from input stream.
         * @param input input stream
         */
        properties(const input_stream_ref& input);

        /**
         * @brief Copy assignment.
         * @param p properties to copy
         * @return reference to this
         */
        properties& operator=(const properties& p);
        /**
         * @brief Move assignment.
         * @param p properties to move
         * @return reference to this
         */
        properties& operator=(properties&& p);

        /**
         * @brief Check if property exists.
         * @param key property key
         * @return true if property exists
         */
        bool exists(const char* key) const
        {
            return m_data.find(key) != m_data.end();
        }

        /**
         * @brief Check if property exists.
         * @param key property key
         * @return true if property exists
         */
        bool exists(const std::string_view& key) const
        {
            return m_data.find(key) != m_data.end();
        }

        /**
         * @brief Check if property exists.
         * @param key property key
         * @return true if property exists
         */
        bool exists(const std::string& key) const
        {
            return m_data.find(key) != m_data.end();
        }

        /**
         * @brief Get property value.
         * @tparam T value type
         * @param key property key
         * @return property value
         */
        template <typename T> T get(const char* key) const
        {
            if (key == nullptr) {
                MGE_THROW(null_pointer) << "Argument 'key' must not be null";
            }
            auto it = m_data.find(key);
            if (it != m_data.end()) {
                return lexical_cast<T>(it->second);
            }
            MGE_THROW(no_such_element) << "No property '" << key << "' found";
        }

        /**
         * @brief Get property value with default.
         * @tparam T value type
         * @tparam D default value type
         * @param key property key
         * @param default_value default value if property not found
         * @return property value or default
         */
        template <typename T, typename D>
        T get(const char* key, const D& default_value) const
        {
            if (key == nullptr) {
                return default_value;
            }

            auto it = m_data.find(key);
            if (it != m_data.end()) {
                return lexical_cast<T>(it->second);
            } else {
                return default_value;
            }
        }
        /**
         * @brief Get property value.
         * @tparam T value type
         * @param key property key
         * @return property value
         */
        template <typename T> T get(std::string_view key) const
        {
            if (key.data() == nullptr) {
                MGE_THROW(null_pointer) << "Argument 'key' must not be null";
            }

            auto it = m_data.find(key);
            if (it != m_data.end()) {
                return lexical_cast<T>(it->second);
            }
            MGE_THROW(no_such_element) << "No property '" << key << "' found";
        }

        /**
         * @brief Get property value with default.
         * @tparam T value type
         * @tparam D default value type
         * @param key property key
         * @param default_value default value if property not found
         * @return property value or default
         */
        template <typename T, typename D>
        T get(std::string_view key, const D& default_value) const
        {
            if (key.data() == nullptr) {
                return default_value;
            }

            auto it = m_data.find(key);
            if (it != m_data.end()) {
                return lexical_cast<T>(it->second);
            } else {
                return default_value;
            }
        }

        /**
         * @brief Get property value.
         * @tparam T value type
         * @param key property key
         * @return property value
         */
        template <typename T> T get(const std::string& key) const
        {
            auto it = m_data.find(key);
            if (it != m_data.end()) {
                return lexical_cast<T>(it->second);
            }
            MGE_THROW(no_such_element) << "No property '" << key << "' found";
        }

        /**
         * @brief Get property value with default.
         * @tparam T value type
         * @tparam D default value type
         * @param key property key
         * @param default_value default value if property not found
         * @return property value or default
         */
        template <typename T, typename D>
        T get(const std::string& key, const D& default_value) const
        {
            auto it = m_data.find(key);
            if (it != m_data.end()) {
                return lexical_cast<T>(it->second);
            } else {
                return default_value;
            }
        }

        /**
         * @brief Set a value.
         *
         * @tparam K key type
         * @tparam T value type
         * @param k key
         * @param value value
         */
        template <typename K, typename T> inline void set(K&& k, T&& value)
        {
            put(std::forward<K>(k), std::forward<T>(value));
        }

        /**
         * @brief Set property value.
         * @tparam T value type
         * @param key property key
         * @param value property value
         */
        template <typename T> inline void put(const char* key, const T& value)
        {
            std::stringstream ss;
            ss << value;
            m_data[key] = ss.str();
        }

        /**
         * @brief Set property value.
         * @tparam T value type
         * @param key property key
         * @param value property value
         */
        template <typename T>
        inline void put(std::string_view key, const T& value)
        {
            std::stringstream ss;
            ss << value;
            m_data[std::string(key)] = ss.str();
        }

        /**
         * @brief Set property value.
         * @tparam T value type
         * @param key property key
         * @param value property value
         */
        template <typename T>
        inline void put(const std::string& key, const T& value)
        {
            std::stringstream ss;
            ss << value;
            m_data[key] = ss.str();
        }

        /**
         * @brief Remove property.
         * @param key property key
         */
        void erase(std::string_view key);
        /**
         * @brief Remove property.
         * @param key property key
         */
        void erase(const std::string& key);

        /**
         * @brief Remove all properties.
         */
        void clear();

        /**
         * @brief Load properties from stream.
         * @param s input stream
         */
        void load(std::istream& s);
        /**
         * @brief Save properties to stream.
         * @param s output stream
         */
        void store(std::ostream& s) const;

        /**
         * @brief Get iterator to beginning.
         * @return iterator
         */
        iterator begin()
        {
            return m_data.begin();
        }
        /**
         * @brief Get iterator to end.
         * @return iterator
         */
        iterator end()
        {
            return m_data.end();
        }
        /**
         * @brief Get const iterator to beginning.
         * @return const iterator
         */
        const_iterator cbegin() const
        {
            return m_data.cbegin();
        }
        /**
         * @brief Get const iterator to end.
         * @return const iterator
         */
        const_iterator cend() const
        {
            return m_data.cend();
        }
        /**
         * @brief Get const iterator to beginning.
         * @return const iterator
         */
        const_iterator begin() const
        {
            return cbegin();
        }
        /**
         * @brief Get const iterator to end.
         * @return const iterator
         */
        const_iterator end() const
        {
            return cend();
        }

        /**
         * @brief Equality comparison.
         * @param p properties to compare
         * @return true if properties are equal
         */
        inline bool operator==(const properties& p) const
        {
            return m_data == p.m_data;
        }

        inline bool operator!=(const properties& p) const
        {
            return m_data != p.m_data;
        }

    private:
        map_type m_data;
    };

} // namespace mge
