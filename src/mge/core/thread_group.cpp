#include "mge/core/thread_group.hpp"
#include "mge/core/crash.hpp"
#include "mge/core/contains.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/log.hpp"

MGE_USE_LOG(THREAD);

namespace mge {

    thread_group::thread_group()
    {}

    thread_group::~thread_group()
    {
        MGE_CRASH_ASSERT(!this_thread_in_group(),
                         "Cannot destroy thread group from thread in group");
    }

    bool
    thread_group::this_thread_in_group()
    {
        thread *this_thread = mge::thread::this_thread();
        if(this_thread == nullptr) {
            return false;
        }

        {
            std::lock_guard<decltype(m_mutex)> guard(m_mutex);
            return contains(m_threads, this_thread);
        }
    }

    void
    thread_group::add_thread(thread *t)
    {
        if(t == thread::this_thread()) {
            MGE_THROW(mge::illegal_argument(),
                      "A thread cannot add itself");
        }
        if(t->group()) {
            MGE_THROW(mge::illegal_argument(),
                      "Thread is already in thread group");
        }

        {
            std::lock_guard<decltype(m_mutex)> guard(m_mutex);
            m_threads.push_back(t);
            t->m_group = this;
        }
    }

    void
    thread_group::remove_thread(thread *t)
    {
        if(t == thread::this_thread()) {
            MGE_THROW(mge::illegal_argument(),
                      "A thread cannot remove itself");
        }
        if(t->m_group != this) {
            MGE_THROW(mge::illegal_argument(),
                      "Thread is not in thread group");
        }
        {
            std::lock_guard<decltype(m_mutex)> guard(m_mutex);
            for(auto it = m_threads.begin(); it!=m_threads.end(); ++it) {
                if(*it == t) {
                    m_threads.erase(it);
                    t->m_group = nullptr;
                    return;
                }
            }
        }
    }

    void
    thread_group::join_all()
    {
        if(this_thread_in_group()) {
            MGE_THROW(mge::illegal_state(),
                      "Cannot join threads in thread group from contained thread");
        }
        {
            std::lock_guard<decltype(m_mutex)> guard(m_mutex);
            MGE_DEBUG_LOG(THREAD) << "Joining group of " << m_threads.size() << " threads";
            int i=1;
            for(const auto t : m_threads) {
                if(t->joinable()) {
                    t->join();
                    MGE_DEBUG_LOG(THREAD) << "Thread " << i << " joined";
                } else {
                    MGE_DEBUG_LOG(THREAD) << "Thread " << i << " not in joinable state";
                }
                ++i;
            }
        }
    }

}
