// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/debugging.hpp"
#include "mge/config.hpp"
#include "mge/core/crash.hpp"

#if MGE_OS_WINDOWS
#    include <windows.h>
#elif MGE_OS_LINUX
#    include <fstream>
#    include <signal.h>
#else
#    error Missing port
#endif

namespace mge {
    void MGECORE_EXPORT breakpoint() noexcept
    {
#if MGE_OS_WINDOWS
        if (IsDebuggerPresent()) {
            DebugBreak();
        } else {
            crash("Calling debugger with no debugger attached");
        }
#elif MGE_OS_LINUX
        if (is_debugger_present()) {
            raise(SIGTRAP);
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
#elif MGE_OS_LINUX
        std::ifstream status("/proc/self/status");
        std::string   line;
        while (std::getline(status, line)) {
            if (line.find("TracerPid:") == 0) {
                int pid = std::stoi(line.substr(10));
                return pid != 0;
            }
        }
        return false;
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