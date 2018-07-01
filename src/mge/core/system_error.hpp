// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/exception.hpp"
namespace mge {
    class system_error : public exception
    {
    public:
        static void clear();
    };

    struct tag_sysrc {};
    typedef error_info<tag_sys
}
