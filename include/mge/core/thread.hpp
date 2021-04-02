#pragma once
#include "mge/config.hpp"
#include "mge/core/dllexport.hpp"

#include <chrono>
#include <thread>

namespace mge {

    class MGECORE_EXPORT thread
    {
    public:
        using id = std::thread::id;
#ifdef MGE_OS_WINDOWS
        using system_id = uint32_t;
#else
#    error missing port
#endif
    };

    namespace this_thread {

        MGECORE_EXPORT mge::thread::system_id system_id();

        inline mge::thread::id get_id() { return ::std::this_thread::get_id(); }

        template <class Rep, class Period>
        inline void
        sleep_for(const std::chrono::duration<Rep, Period> &sleep_duration)
        {
            ::std::this_thread::sleep_for(sleep_duration);
        }
    } // namespace this_thread

} // namespace mge