// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/types.hpp"
#include <functional>
#include <iostream>
#include <thread>
#include <utility>

namespace mge {

    class thread_group;

    /**
     * Thread class. Enhances std::thread by convenience methods.
     */
    class MGE_CORE_EXPORT thread : noncopyable,
                                   public std::enable_shared_from_this<thread>
    {
    public:
        /// Type of 'running' thread. Usually @c std::thread.
        using running_thread_t = std::thread;
        /// Native thread handle type.
        using native_handle_type = running_thread_t::native_handle_type;
        /// Thread id type.
        using id = running_thread_t::id;

        /**
         * Constructor.
         */
        explicit thread();

        /**
         * Destructor.
         */
        virtual ~thread();

        /**
         * Get current thread.
         * @return current thread, @c nullptr in case the current thread is
         * unmanaged
         */
        static thread *this_thread();

        /**
         * Return thread group of thread.
         * @return thread group, nullpointer if thread has no group
         */
        thread_group *group() const;

        /**
         * Starts the thread and calls this object's @c run method.
         */
        void start();

        /**
         * Starts the thread and runs the provided function in the
         * thread.
         * @param f thread function
         */
        inline void start(const std::function<void()> &f)
        {
            if (!f) {
                MGE_THROW(mge::illegal_argument)
                    << "Invalid function parameter for thread start";
            }

            if (joinable()) {
                MGE_THROW(mge::illegal_state)
                    << "This thread has already been started";
            }

            m_running_thread = running_thread_t([f, this] {
                try {
                    this->on_start();
                    f();
                    this->on_finish();
                } catch (...) {
                    this->on_exception(std::current_exception());
                }
            });
        }

        /**
         * Run thread logic. The default method does nothing.
         */
        virtual void run();

        /**
         * Joins the thread.
         */
        void join();

        /**
         * Detaches the thread.
         */
        void detach();

        /**
         * Return thread id.
         * @return thread id
         */
        id get_id() const;

        /**
         * Return the thread's native handle.
         * @return native handle
         */
        native_handle_type native_handle();

        /**
         * Provides a hint on the number of threads that can run
         * concurrently.
         * @return number of threads that can run concurrently,
         *   0 if it cannot be determined.
         */
        static unsigned int hardware_concurrency() noexcept;

        /**
         * Returns @c true if thread can be joined. To be joinable,
         * the thread must run and be not equal to this thread (a
         * thread cannot join itself).
         * @return @c true if thread can be joined
         */
        bool joinable();

    private:
        friend class stacktrace;
        friend class thread_group;

        void             on_start();
        void             on_finish();
        void             on_exception(const std::exception_ptr &eptr);
        running_thread_t m_running_thread;
        thread_group *   m_group;
    };

    namespace this_thread {
        inline mge::thread::id get_id() { return ::std::this_thread::get_id(); }

        template <class Rep, class Period>
        inline void
        sleep_for(const std::chrono::duration<Rep, Period> &sleep_duration)
        {
            ::std::this_thread::sleep_for(sleep_duration);
        }
    } // namespace this_thread

} // namespace mge
