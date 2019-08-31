// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/thread.hpp"
#include "mge/core/crash.hpp"
#include "mge/core/log.hpp"

MGE_DEFINE_LOG(THREAD)

namespace mge {



    static thread_local thread *t_this_thread;

    thread::thread()
        :m_group(nullptr)
    {}


    thread::~thread()
    {
        MGE_CRASH_ASSERT(t_this_thread != this,
                         "Cannot destroy thread object from itself");
        MGE_CRASH_ASSERT(m_group == nullptr,
                         "Thread is still contained in thread group");
    }

    thread_group *
    thread::group() const
    {
        return m_group;
    }

    thread*
    thread::this_thread()
    {
        return t_this_thread;
    }

    void
    thread::start()
    {
        start([this]{ this->run(); });
    }

    thread::id
    thread::get_id() const
    {
        return m_running_thread.get_id();
    }

    thread::native_handle_type
    thread::native_handle()
    {
        return m_running_thread.native_handle();
    }

    unsigned int
    thread::hardware_concurrency() noexcept
    {
        return running_thread_t::hardware_concurrency();
    }

    void
    thread::detach()
    {
        m_running_thread.detach();
    }

    void
    thread::run()
    {}

    void
    thread::on_start()
    {
        t_this_thread = this;
    }

    void
    thread::on_finish()
    {
        if(t_this_thread != this) {
            crash("Inconsistency: thread object no longer assigned on exit");
        }
    }

    void
    thread::on_exception(const std::exception_ptr &eptr)
    {
        try {
            std::rethrow_exception(eptr);
        } catch(const mge::exception& e) {
            MGE_ERROR_LOG(THREAD) << "Thread aborted with exception:" << std::endl << e.details() << std::endl;
            throw;
        } catch(const std::exception& e) {
            MGE_ERROR_LOG(THREAD) << "Thread aborted with exception:" << e.what() << std::endl;
            throw;
        } catch(...) {
            MGE_ERROR_LOG(THREAD) << "Thread aborted non-std exception" << std::endl;
            throw;
        }
    }

    bool
    thread::joinable()
    {
        return m_running_thread.joinable();
    }

    void
    thread::join()
    {
        m_running_thread.join();
    }

}
