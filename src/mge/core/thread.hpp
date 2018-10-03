// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/types.hpp"

#include <thread>
#include <iostream>
#include <utility>
#include <functional>


namespace mge {

    /**
     * Thread class. Enhances std::thread by convenience methods.
     */
    class MGE_CORE_EXPORT thread
            : noncopyable
            , public std::enable_shared_from_this<thread>
    {
    public:
        /// Type of 'running' thread. Usually @c std::thread.
        typedef std::thread running_thread_t;
        /// Native thread handle type.
        typedef running_thread_t::native_handle_type native_handle_type;
        /// Thread id type.
        typedef running_thread_t::id id;

        /**
         * Constructor.
         */
        explicit thread();

        /**
         * Move constructor.
         * @param t moved instance
         */
        thread(thread&& t);

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

        inline void start(const std::function<void()>& f)
        {

        }
    private:
        void on_start();
        void on_finish();
        void on_exception();
        running_thread_t m_running_thread;
    };
}
