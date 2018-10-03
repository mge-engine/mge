/*
 * Copyright (c) 2017 by Alexander Schroeder
 */
#pragma once

#include "mge/core/dllexport.hpp"
#include "mge/core/types.hpp"
#include "mge/core/log_severity.hpp"
#include "mge/core/thread.hpp"

#include <chrono>
#include <thread>

namespace mge {

    class MGE_CORE_EXPORT log_record
    {
    public:
        typedef std::thread::id thread_id_type;
        log_record();
        log_record(const log_record& record);
        ~log_record() = default;

        log_record& operator = (const log_record& record);

        inline const char *category() const { return &m_category[0]; }
        inline log_severity severity() const { return m_severity; }
        inline const std::chrono::system_clock::time_point timestamp() const
            { return m_timestamp; }
        inline const char *message() const { return &m_messageline[0]; }
        inline thread_id_type  thread() const { return m_thread; }
        void init(const char *category,
                  log_severity severity,
                  thread_id_type thread_id);

    private:
        friend class log;

        log_severity m_severity;
        std::chrono::system_clock::time_point m_timestamp;
        thread_id_type m_thread;
        char m_category[256];
        char m_messageline[512];
    };
}
