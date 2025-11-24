// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/core/checked_cast.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/is_associative_container.hpp"
#include "mge/core/is_container.hpp"
#include "mge/core/lexical_cast.hpp"
#include "nlohmann/json.hpp"
#include <type_traits>

namespace mge {
    namespace json = ::nlohmann;
}

namespace mge {
    template <typename T> inline void from_json(const json::json& j, T& v)
    {
        v = j.get<T>();
    }

    template <typename T>
        requires std::is_integral<T>::value || std::is_floating_point<T>::value
    inline void from_json(const json::json& j, T& v)
    {
        if (j.is_number_integer()) {
            int64_t tmp = j.get<int64_t>();
            v = mge::checked_cast<T>(tmp);
        } else if (j.is_number_unsigned()) {
            uint64_t tmp = j.get<uint64_t>();
            v = mge::checked_cast<T>(tmp);
        } else if (j.is_number_float()) {
            v = mge::checked_cast<T>(j.get<double>());
        } else if (j.is_boolean()) {
            v = j.get<bool>() ? 1 : 0;
        } else if (j.is_string()) {
            v = mge::lexical_cast<T>(j.get<std::string>());
        } else {
            MGE_THROW(mge::bad_cast)
                << "Cannot convert JSON value " << j.dump()
                << " to integral type " << typeid(T).name();
        }
    }

    template <typename T>
        requires mge::is_container<T> && ((!std::is_same_v<T, std::string>) &&
                                          (!std::is_same_v<T, std::wstring>) &&
                                          (!is_associative_container<T>))
    inline void from_json(const json::json& j, T& v)
    {
        if (j.is_array()) {
            v.clear();
            for (const auto& e : j) {
                typename T::value_type tmp;
                from_json(e, tmp);
                v.push_back(tmp);
            }
        } else {
            MGE_THROW(mge::bad_cast)
                << "Cannot convert JSON value " << j.dump()
                << " to container type " << typeid(T).name();
        }
    }

    template <typename T>
        requires mge::is_associative_container<T>
    inline void from_json(const json::json& j, T& v)
    {
        if (j.is_object()) {
            for (const auto& [key, value] : j.items()) {
                typename T::key_type    tkey;
                typename T::mapped_type tvalue;
                from_json(key, tkey);
                from_json(value, tvalue);
                v.emplace(tkey, tvalue);
            }
        } else {
            MGE_THROW(mge::bad_cast)
                << "Cannot convert JSON value " << j.dump()
                << " to container type " << typeid(T).name();
        }
    }

    template <>
    inline void from_json<std::string>(const json::json& j, std::string& v)
    {
        if (j.is_string()) {
            v = j.get<std::string>();
        } else {
            v = j.dump();
        }
    }

    template <typename T> inline void to_json(json::json& j, const T& t)
    {
        j = t;
    }

    template <typename T>
        requires mge::is_container<T> && ((!std::is_same_v<T, std::string>) &&
                                          (!std::is_same_v<T, std::wstring>) &&
                                          (!is_associative_container<T>))
    inline void to_json(json::json& j, const T& v)
    {
        for (const auto& e : v) {
            to_json(j.emplace_back(), e);
        }
    }

    template <typename T>
        requires mge::is_associative_container<T> &&
                 std::is_same_v<typename T::key_type, std::string>
    inline void to_json(json::json& j, const T& v)
    {
        for (const auto& [key, value] : v) {
            to_json(j[key], value);
        }
    }

    inline void to_json(json::json& j, const char* v)
    {
        j = std::string(v);
    }

} // namespace mge