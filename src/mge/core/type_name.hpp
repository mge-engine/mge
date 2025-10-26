// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/config.hpp"
#include "mge/core/dllexport.hpp"
#include <source_location>
#include <string>
#include <typeinfo>

namespace mge {

    /**
     * @brief Get the type name of the supplied type information.
     *
     * @param ti  type info
     * @return type name
     */
    MGECORE_EXPORT std::string type_name(const std::type_info& ti);
    /**
     * @brief Base type name, i.e. type name without name space.
     *
     * @param ti type info
     * @return base type name
     */
    MGECORE_EXPORT std::string base_type_name(const std::type_info& ti);
    /**
     * @brief Namespace name.
     *
     * @param ti type info
     * @return namespace name
     */
    MGECORE_EXPORT std::string namespace_name(const std::type_info& ti);

    namespace details {
        struct sview
        {
            const char* str_;
            std::size_t sz_;
        };

        template <typename T> inline constexpr auto name() noexcept
        {
#ifdef MGE_COMPILER_MSVC
            if constexpr (sizeof(__FUNCSIG__) >= 55 && __FUNCSIG__[32] == 'c' &&
                          __FUNCSIG__[33] == 'l' && __FUNCSIG__[34] == 'a' &&
                          __FUNCSIG__[35] == 's' && __FUNCSIG__[36] == 's' &&
                          __FUNCSIG__[37] == ' ') {
                return sview{__FUNCSIG__ + 38, sizeof(__FUNCSIG__) - 55};
            }
            if constexpr (sizeof(__FUNCSIG__) >= 56 && __FUNCSIG__[32] == 's' &&
                          __FUNCSIG__[33] == 't' && __FUNCSIG__[34] == 'r' &&
                          __FUNCSIG__[35] == 'u' && __FUNCSIG__[36] == 'c' &&
                          __FUNCSIG__[37] == 't' && __FUNCSIG__[38] == ' ') {
                return sview{__FUNCSIG__ + 39, sizeof(__FUNCSIG__) - 56};
            }
            return sview{__FUNCSIG__ + 32, sizeof(__FUNCSIG__) - 49};
#else
#    error Not implemented
#endif
        }
    } // namespace details

    /**
     * @brief Get type name of type.
     *
     * @tparam T inspected type
     * @return type name
     */
    template <typename T> consteval std::string_view type_name()
    {
        auto n = details::name<T>();
        return std::string_view(n.str_, n.sz_);
    }

    /**
     * @brief Get base name of type.
     *
     * @tparam T inspected type
     * @return base type name without name space
     */
    template <typename T> inline std::string base_type_name()
    {
        return base_type_name(typeid(T));
    }
    /**
     * @brief Get namespace name of type.
     *
     * @tparam T inspected type
     * @return namespace name
     */
    template <typename T> inline std::string namespace_name()
    {
        return namespace_name(typeid(T));
    }

} // namespace mge
