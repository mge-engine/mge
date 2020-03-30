// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/debug_break.hpp"
#include "mge/config.hpp"

#ifdef MGE_OS_WINDOWS
#    include <windows.h>
#endif

namespace mge {
    void debug_break()
    {
#ifdef MGE_OS_WINDOWS
        DebugBreak();
#else
#    error Not implemented.
#endif
    }
} // namespace mge