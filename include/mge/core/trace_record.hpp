// mge - Modern Game Engine
// Copyright (c) 2019 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/clock.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/trace_level.hpp"
#include <string_view>

namespace mge {

    struct MGECORE_EXPORT trace_record
    {
    public:
        clock::time_point time;
        uint32_t          sequence;
        trace_level       level;
        std::string_view  message;
    };

} // namespace mge