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
    {}

    thread::thread(thread&& t)
        :m_running_thread(std::move(t.m_running_thread))
    {}

    thread::~thread()
    {
        if (t_this_thread == this) {
            crash("Cannot destroy thread object from itself");
        }
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
        // TODO: encapsulate std::system_error
        m_running_thread.join();
    }

}
