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

            static consteval auto name()
            {
#if defined(MGE_COMPILER_MSVC)
                std::string_view prefix("::$h$type_name$<");
                std::string_view func_name(
                    std::source_location::current().function_name());

                const auto pos = func_name.find(prefix);
                if (pos == std::string::npos) {
                    return std::string_view("???");
                }

                std::remove_const_t<decltype(pos)> end =
                    pos + prefix.size() + 1;
                int level = 1;
                while (level > 0) {
                    if (end >= func_name.size()) {
                        return std::string_view("???");
                    }
                    if (func_name[end] == '<') {
                        ++level;
                    } else if (func_name[end] == '>') {
                        --level;
                    }
                    if (level > 0) {
                        ++end;
                    }
                }

                while (end > 0 && func_name[end - 1] == ' ') {
                    --end;
                }

                auto n = func_name.substr(pos + prefix.size(),
                                          end - pos - prefix.size());
                if (n.starts_with("class "))
                    n = n.substr(6);
                else if (n.starts_with("struct "))
                    n = n.substr(7);
                return n;
#elif defined(MGE_COMPILER_GCC)
                std::string_view func_name(
                    std::source_location::current().function_name());
                std::string_view prefix("[with T = ");
                const auto       pos = func_name.find(prefix);
                if (pos == std::string::npos) {
                    return std::string_view("???");
                }
                auto end = func_name.find(']', pos + prefix.size());
                if (end == std::string::npos) {
                    return std::string_view("???");
                }
                auto n = func_name.substr(pos + prefix.size(),
                                          end - pos - prefix.size());
                return n;
#else
#    error Missing port
#endif
            }
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
