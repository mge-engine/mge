// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/core/dllexport.hpp"
#include "mge/core/types.hpp"
#include "mge/core/log_severity.hpp"
#include "mge/core/thread.hpp"

#include <chrono>
#include <thread>

namespace mge {

    struct MGE_CORE_EXPORT log_record
    {
        typedef std::thread::id thread_id_type;
        typedef std::chrono::system_clock::time_point timestamp_type;

        log_record& set_message(const char *m)
        {
            message = m;
            return *this;
        }

        log_severity severity;
        timestamp_type timestamp;
        thread_id_type thread_id;
        const char *category;
        const char *message;
    };
}
