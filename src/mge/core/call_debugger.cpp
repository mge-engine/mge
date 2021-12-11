#include "mge/core/call_debugger.hpp"
#include "mge/config.hpp"

#if defined(MGE_OS_WINDOWS)
#    include <windows.h>
#else
#    error Missing port
#endif

namespace mge {
    void call_debugger()
    {
#if defined(MGE_OS_WINDOWS)
        DebugBreak();
#else
#    error Missing port
#endif
    }
} // namespace mge