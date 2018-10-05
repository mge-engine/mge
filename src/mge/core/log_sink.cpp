// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/log_sink.hpp"

namespace mge {

    log_sink::log_sink(bool synchronized)
        :m_synchronized(synchronized)
    {}

    void
    log_sink::publish(const log_record& r)
    {
        if (m_synchronized) {
            std::lock_guard<std::mutex> guard(m_lock);
            on_publish(r);
        } else {
            on_publish(r);
        }
    }

    MGE_REGISTER_COMPONENT(log_sink);

}
