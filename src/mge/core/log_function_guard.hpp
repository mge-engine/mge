// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/log.hpp"

#include <iostream>
namespace mge {

    class log_function_guard
    {
    public:
        log_function_guard(const char *function, log *l)
            : m_function(function), m_log(l)
        {
            if (m_log) {

                log_context(m_log->begin_entry(log_severity::DEBUG_SEVERITY))
                        .stream()
                    << "-> " << m_function;
            }
        }

        ~log_function_guard()
        {
            if (m_log) {
                log_context(m_log->begin_entry(log_severity::DEBUG_SEVERITY))
                        .stream()
                    << "<- " << m_function;
            }
        }

    private:
        const char *m_function;
        log *       m_log;
    };
} // namespace mge

#define MGE_LOG_FUNCTION(TOPIC)                                                \
    mge::log_function_guard __log_guard(                                       \
        MGE_FUNCTION_SIGNATURE,                                                \
        ::log_instance_##TOPIC().enabled(mge::log_severity::DEBUG_SEVERITY)    \
            ? &::log_instance_##TOPIC()                                        \
            : nullptr)
