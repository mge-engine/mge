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

        properties();
        properties(const properties& p);
        properties(properties&& p);
        properties(std::istream& input);
        properties(const input_stream_ref& input);

        properties& operator=(const properties& p);
        properties& operator=(properties&& p);

        bool exists(const char* key) const
        {
            return m_data.find(key) != m_data.end();
        }

        bool exists(const std::string_view& key) const
        {
            return m_data.find(key) != m_data.end();
        }

        bool exists(const std::string& key) const
        {
            return m_data.find(key) != m_data.end();
        }

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

        template <typename T> T get(const std::string& key) const
        {
            auto it = m_data.find(key);
            if (it != m_data.end()) {
                return lexical_cast<T>(it->second);
            }
            MGE_THROW(no_such_element) << "No property '" << key << "' found";
        }

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

        template <typename T> inline void put(const char* key, const T& value)
        {
            std::stringstream ss;
            ss << value;
            m_data[key] = ss.str();
        }

        template <typename T>
        inline void put(std::string_view key, const T& value)
        {
            std::stringstream ss;
            ss << value;
            m_data[std::string(key)] = ss.str();
        }

        template <typename T>
        inline void put(const std::string& key, const T& value)
        {
            std::stringstream ss;
            ss << value;
            m_data[key] = ss.str();
        }

        void erase(std::string_view key);
        void erase(const std::string& key);

        void clear();

        void load(std::istream& s);
        void store(std::ostream& s) const;

        iterator begin()
        {
            return m_data.begin();
        }
        iterator end()
        {
            return m_data.end();
        }
        const_iterator cbegin() const
        {
            return m_data.cbegin();
        }
        const_iterator cend() const
        {
            return m_data.cend();
        }
        const_iterator begin() const
        {
            return cbegin();
        }
        const_iterator end() const
        {
            return cend();
        }

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
