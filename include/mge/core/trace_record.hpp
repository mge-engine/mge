// mge - Modern Game Engine
// Copyright (c) 2019 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/clock.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/trace_level.hpp"

namespace mge {

    class MGECORE_EXPORT trace_record
    {
    public:
        clock::time_point time;
    };
} // namespace mge