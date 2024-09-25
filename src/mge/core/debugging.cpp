// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/debugging.hpp"
#include "mge/config.hpp"
#include "mge/core/crash.hpp"

#if defined(MGE_OS_WINDOWS)
#    include <windows.h>
#else
#    error Missing port
#endif

namespace mge {
    void MGECORE_EXPORT breakpoint() noexcept
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

    bool MGECORE_EXPORT is_debugger_present() noexcept
    {
#if defined(MGE_OS_WINDOWS)
        try {
            return IsDebuggerPresent();
        } catch (...) {
            return false;
        }
#else
#    error Missing port
#endif
    }

    void MGECORE_EXPORT breakpoint_if_debugging() noexcept
    {
        if (is_debugger_present()) {
            breakpoint();
        }
    }

} // namespace mge