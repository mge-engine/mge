// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/task.hpp"
#include "mge/core/crash.hpp"
#include "mge/core/stdexceptions.hpp"
namespace mge {
    task::task(const std::function<void()>& f)
        :m_function(f)
    {
        if (!f) {
           MGE_THROW(illegal_argument(),
                     "Invalid function to call in task");
        }
    }

    task::~task()
    {}

    void
    task::run()
    {
        try {
            if(m_function) {
                m_function();
                m_result.set_value();
            } else {
                MGE_THROW(mge::illegal_state(),
                          "Invalid function to call in task");
            }
        } catch(...) {
            try {
                m_result.set_exception(std::current_exception());
            } catch(...) {
                mge::crash("Exception forwarding current exception from task run");
            }
        }
    }

    void
    task::wait()
    {
        m_result.get_future().get();
    }

}
