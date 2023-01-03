// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/application/dllexport.hpp"
#include <cstdint>

namespace mge {
    /**
     * @brief Context of current loop.
     *
     */
    class MGEAPPLICATION_EXPORT loop_context
    {
    public:
        uint64_t cycle;
        double   delta;
        double   peek;
    };
} // namespace mge