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

    namespace {

        template <typename T> struct $h$type_name$
        {
            static consteval auto name()
            {
                std::string_view prefix("::$h$type_name$<");
                std::string_view func_name(
                    std::source_location::current().function_name());

                const auto pos = func_name.find(prefix);
                if (pos == std::string::npos) {
                    return std::string_view("xxx");
                }
#if MGE_COMPILER_GCC
                return std::string_view("<unsupported>");
#else
                std::remove_const_t<decltype(pos)> end =
                    pos + prefix.size() + 1;
                int level = 1;
                while (level > 0) {
                    ++end;
                    if (func_name[end] == '<') {
                        ++level;
                    } else if (func_name[end] == '>') {
                        --level;
                    }
                    if (end >= func_name.size()) {
                        return std::string_view("???");
                    }
                }

                while (func_name[end - 1] == ' ') {
                    --end;
                }

                auto n = func_name.substr(pos + prefix.size(),
                                          end - pos - prefix.size());
                if (n.starts_with("class "))
                    n = n.substr(6);
                else if (n.starts_with("struct "))
                    n = n.substr(7);
                return n;
#endif
            }
        };
    } // namespace

    /**
     * @brief Get type name of type.
     *
     * @tparam T inspected type
     * @return type name
     */
    template <typename T> consteval std::string_view type_name()
    {
        return $h$type_name$<T>::name();
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
