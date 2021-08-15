// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/core/dllexport.hpp"
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

    /**
     * @brief Get type name of type.
     *
     * @tparam T inspected type
     * @return type name
     */
    template <typename T> inline std::string type_name()
    {
        return type_name(typeid(T));
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
