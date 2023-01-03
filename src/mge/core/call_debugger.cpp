// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/call_debugger.hpp"
#include "mge/config.hpp"
#include "mge/core/crash.hpp"

#if defined(MGE_OS_WINDOWS)
#    include <windows.h>
#else
#    error Missing port
#endif

namespace mge {
    void call_debugger()
    {
#if defined(MGE_OS_WINDOWS)
        if (IsDebuggerPresent()) {
            DebugBreak();
        } else {
            crash("Calling debugger with no debugger attached");
        }
#else
#    error Missing port
#endif
    }
} // namespace mge