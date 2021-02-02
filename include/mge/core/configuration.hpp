// mge - Modern Game Engine
// Copyright (c) 2019 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"

namespace mge {

    class basic_parameter;
    class MGECORE_EXPORT configuration
    {
    public:
        static void register_parameter(basic_parameter &p);
        static void unregister_parameter(basic_parameter &p);
    };

} // namespace mge