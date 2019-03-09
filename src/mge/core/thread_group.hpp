#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/thread.hpp"

#include <vector>

namespace mge {

    class MGE_CORE_EXPORT thread_group
            : public noncopyable
    {
    public:
        thread_group();
        ~thread_group();

        void add_thread(thread* t);
        void remove_thread(thread *t);
    private:
        bool this_thread_in_group();

        std::vector<thread *> m_threads;
        std::mutex            m_mutex;
    };
}
