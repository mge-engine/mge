// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
//#include "mge/core/input_stream.hpp"
//#include "mge/core/lexical_cast.hpp"
#include "mge/core/memory.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/string_pool.hpp"

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
        properties();
        properties(const properties& p);
        properties(properties&& p);
        // properties(const input_stream_ref& input);

        properties& operator=(const properties& p);
        properties& operator=(properties&& p);

        bool exists(const char* key) const { return m_data.find(key) != m_data.end(); }

        bool exists(const std::string& key) const { return m_data.find(key) != m_data.end(); }

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

        template <typename T, typename D> T get(const char* key, const D& default_value) const
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

        template <typename T> inline void put(const char* key, const T& value)
        {
            std::stringstream ss;
            ss << value;
            m_data[key] = ss.str();
        }

        template <typename T> inline void put(const std::string& key, const T& value)
        {
            std::stringstream ss;
            ss << value;
            m_data[key] = ss.str();
        }

        void erase(const char* key);
        void erase(const std::string& key);

        void clear();

        void load(std::istream& s);

    private:
        string_pool                                            m_pool;
        std::unordered_map<std::string_view, std::string_view> m_data;
    };

} // namespace mge
