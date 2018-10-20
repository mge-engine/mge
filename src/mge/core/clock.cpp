#include "mge/core/clock.hpp"
#include "mge/config.hpp"
#include <chrono>
#ifdef MGE_OS_WINDOWS
#include <windows.h>
#endif

namespace mge {

    static auto epoch = std::chrono::high_resolution_clock::now();

    uint64_t clock::now()
    {
        auto since_epoch = std::chrono::high_resolution_clock::now() - epoch;
        return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(since_epoch).count());
    }

    uint64_t clock::tick()
    {
        uint64_t result = 0;
#ifdef MGE_OS_WINDOWS
        QueryPerformanceCounter((LARGE_INTEGER*)&result);
#else
#  error missing port
#endif
        return result;
    }

    uint64_t clock::ticks_per_second()
    {
        uint64_t result = 0;
#ifdef MGE_OS_WINDOWS
        QueryPerformanceFrequency((LARGE_INTEGER *)&result);
#else
#   error missing port
#endif
        return result;
    }

}
