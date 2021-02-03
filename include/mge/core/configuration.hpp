// mge - Modern Game Engine
// Copyright (c) 2019 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include <string_view>
namespace mge {

    class basic_parameter;
    class MGECORE_EXPORT configuration
    {
    public:
        static void register_parameter(basic_parameter &p);
        static void unregister_parameter(basic_parameter &p);

        static basic_parameter &find_parameter(std::string_view section,
                                               std::string_view name);
    };

} // namespace mge