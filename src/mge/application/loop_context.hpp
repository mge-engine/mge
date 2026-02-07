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
        /** @brief Frame counter */
        uint64_t cycle;
        /** @brief Delta time in seconds since last update */
        double   delta;
        /** @brief Peek value for interpolation [0,1) */
        double   peek;
    };
} // namespace mge