// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/debugging.hpp"
#include "mge/config.hpp"
#include "mge/core/crash.hpp"

#if defined(MGE_OS_WINDOWS)
#    include <windows.h>
#elif defined(MGE_OS_LINUX)
#    include <signal.h>
#    include <fstream>
#    include <string>
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
#elif defined(MGE_OS_LINUX)
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
#elif defined(MGE_OS_LINUX)
        // Check /proc/self/status for TracerPid field
        // If TracerPid is non-zero, a debugger/tracer is attached
        try {
            std::ifstream status_file("/proc/self/status");
            std::string line;
            while (std::getline(status_file, line)) {
                if (line.compare(0, 10, "TracerPid:") == 0) {
                    // Extract the PID after "TracerPid:"
                    size_t pos = line.find_first_not_of(" \t", 10);
                    if (pos != std::string::npos) {
                        int tracer_pid = std::stoi(line.substr(pos));
                        return tracer_pid != 0;
                    }
                }
            }
        } catch (...) {
            // If anything goes wrong, assume no debugger
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