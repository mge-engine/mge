#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/types.hpp"
namespace mge {
    class MGE_CORE_EXPORT clock
    {
    public:
        static uint64_t now();
        static uint64_t tick();
        static uint64_t ticks_per_second();
    };
}
