#include "mge/core/thread.hpp"

#ifdef MGE_OS_WINDOWS
#    include <windows.h>
#endif
namespace mge {
    namespace this_thread {
        mge::thread::system_id system_id() { return GetCurrentThreadId(); }
    } // namespace this_thread
} // namespace mge