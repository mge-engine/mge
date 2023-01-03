// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/thread.hpp"
#include <atomic>
#include <sstream>

#ifdef MGE_OS_WINDOWS
#    include <windows.h>
#endif
namespace mge {

    static thread_local thread*  t_this_thread;
    static std::atomic<uint32_t> s_namegen_counter;

    static inline auto gen_thread_name()
    {
        std::ostringstream ss;
        ss << "thread-" << ++s_namegen_counter;
        return ss.str();
    }

    thread::thread()
        : m_name(gen_thread_name())
        , m_group(nullptr)
    {}

    thread::thread(const std::string& name, thread_group* group)
        : m_name(name)
        , m_group(group)
    {}

    thread::~thread() {}

    thread::id thread::get_id() const { return m_running_thread.get_id(); }

    uint32_t thread::hardware_concurrency()
    {
        return running_thread_type::hardware_concurrency();
    }

    void thread::on_start() { t_this_thread = this; }

    void thread::on_finish()
    {
        assert_this_thread_is_this_thread();
        t_this_thread = nullptr;
    }

    void thread::on_exception(const std::exception_ptr& eptr)
    {
        assert_this_thread_is_this_thread();
        t_this_thread = nullptr;
    }

    void thread::run() {}

    void thread::assert_this_thread_is_this_thread()
    {
        if (t_this_thread != this) {
            // todo: crash because thread inconsistency
        }
    }

    void thread::detach() { m_running_thread.detach(); }

    void thread::join() { m_running_thread.join(); }

    bool thread::joinable() const noexcept
    {
        return m_running_thread.joinable();
    }

    namespace this_thread {
        mge::thread::system_id system_id() { return GetCurrentThreadId(); }
    } // namespace this_thread
} // namespace mge