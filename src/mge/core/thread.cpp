// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/thread.hpp"

namespace mge {

    static thread_local thread *t_this_thread;

    thread::thread()
    {}

    thread::thread(thread&& t)
        :m_running_thread(std::move(t.m_running_thread))
    {}

    thread::~thread()
    {}

    thread*
    thread::this_thread()
    {
        return t_this_thread;
    }
}
