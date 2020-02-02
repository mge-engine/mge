#include "mge/config.hpp"
#include "mge/core/debug_break.hpp"

#ifdef MGE_OS_WINDOWS
#  include <windows.h>
#endif

namespace mge {
    void debug_break()
    {
#ifdef MGE_OS_WINDOWS
        DebugBreak();
#else
#  error Not implemented.
#endif
    }
}