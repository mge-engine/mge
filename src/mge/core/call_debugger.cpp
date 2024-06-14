// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/call_debugger.hpp"
#include "mge/config.hpp"
#include "mge/core/crash.hpp"

#if defined(MGE_OS_WINDOWS)
#    include <windows.h>
#endif

#if defined(MGE_OS_MACOSX)
#    include <signal.h>
#    include <sys/sysctl.h>
#    include <unistd.h>
#endif

namespace mge {
    void MGECORE_EXPORT call_debugger()
    {
#if defined(MGE_OS_WINDOWS)
        if (IsDebuggerPresent()) {
            DebugBreak();
        } else {
            crash("Calling debugger with no debugger attached");
        }
#elif defined(MGE_OS_MACOSX)
        int               mib[4];
        struct kinfo_proc info;
        size_t            size = sizeof(info);

        // Set up the mib (Management Information Base) array
        mib[0] = CTL_KERN;
        mib[1] = KERN_PROC;
        mib[2] = KERN_PROC_PID;
        mib[3] = getpid();

        // Get the process information
        if (sysctl(mib, 4, &info, &size, nullptr, 0) == 0) {
            // Check if the process has a debugger attached
            if (info.kp_proc.p_flag & P_TRACED) {
                // Debugger is present
                // Add your code here
                kill(getpid(), SIGTRAP);
            } else {
                // No debugger attached
                crash("Calling debugger with no debugger attached");
            }
        } else {
            crash("Calling debugger failed (cannot get process information)");
        }
#else
#    error Missing port
#endif
    }
} // namespace mge