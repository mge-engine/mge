// mge - Modern Game Engine
// Copyright (c) 2019 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/core/dllexport.hpp"
#include <string>
#include <typeinfo>

namespace mge {

    MGECORE_EXPORT std::string type_name(const std::type_info &ti);
    MGECORE_EXPORT std::string base_type_name(const std::type_info &ti);
    MGECORE_EXPORT std::string namespace_name(const std::type_info &ti);

    template <typename T> inline std::string type_name()
    {
        return type_name(typeid(T));
    }

    template <typename T> inline std::string base_type_name()
    {
        return base_type_name(typeid(T));
    }

    template <typename T> inline std::string namespace_name()
    {
        return namespace_name(typeid(T));
    }
} // namespace mge
