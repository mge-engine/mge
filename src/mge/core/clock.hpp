// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/types.hpp"
namespace mge {

    /**
     * Raw clock that counts time spans since the program start.

     * In a lot of places, a raw clock counting in ticks or nanoseconds is
     required, and
     * easier to use than the time-unit based clocks of @c std::chrono. Also,
     this
     * implementation can decouple from a low precision implementation of
     * @c std::chrono::high_resolution_clock if needed.
     */
    class MGE_CORE_EXPORT clock
    {
    public:
        /**
         * Get time in nanoseconds since program start.
         * @return time in nanoseconds, resolution is system dependend
         */
        static uint64_t now();
        /**
         * Get current number of the high resolution tick counter.
         * @return current ticks
         */
        static uint64_t tick();

        /**
         * Get number of ticks per second.
         * @return number of ticks per second
         */
        static uint64_t ticks_per_second();
    };
} // namespace mge
