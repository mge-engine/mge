// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/clock.hpp"
#include "mge/config.hpp"
#include "mge/core/unlikely.hpp"
#include <chrono>
#ifdef MGE_OS_WINDOWS
#    include <windows.h>
#endif

namespace mge {

    static uint64_t s_epoch = 0;

    uint64_t clock::now()
    {
        uint64_t current_epoch = s_epoch;
        if (current_epoch) {
            uint64_t current_now =
                static_cast<uint64_t>(std::chrono::high_resolution_clock::now()
                                          .time_since_epoch()
                                          .count());
            return current_now - current_epoch;
        }

        s_epoch =
            static_cast<uint64_t>(std::chrono::high_resolution_clock::now()
                                      .time_since_epoch()
                                      .count());
        return 0;
    }

    uint64_t clock::tick()
    {
        uint64_t result = 0;
#ifdef MGE_OS_WINDOWS
        QueryPerformanceCounter((LARGE_INTEGER *)&result);
#else
#    error missing port
#endif
        return result;
    }

    uint64_t clock::ticks_per_second()
    {
        uint64_t result = 0;
#ifdef MGE_OS_WINDOWS
        QueryPerformanceFrequency((LARGE_INTEGER *)&result);
#else
#    error missing port
#endif
        return result;
    }

} // namespace mge
