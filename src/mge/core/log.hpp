// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/log_record.hpp"
#include "mge/core/singleton.hpp"

#include <sstream>
#include <iomanip>

namespace mge {

    class log;
    class log_context;

    class MGE_CORE_EXPORT log_context
    {
    public:
        inline log_context(log& l)
            :m_log(l)
        {}
        inline ~log_context();
        inline std::ostream& stream();
    private:
        log& m_log;
    };

    class MGE_CORE_EXPORT log
            : public noncopyable
    {
    public:
        log(const char *topic);
        ~log();

        log& set_severity(log_severity s);
        static bool enabled(const char * topic, log_severity s);
    private:
        friend class log_context;

        void flush_current_record();
        void publish(const log_record& r);

        log_record        m_log_record;
        std::stringstream m_message;
    };

    inline
    log_context::~log_context()
    {
        m_log.flush_current_record();
    }

    inline std::ostream&
    log_context::stream()
    {
        return m_log.m_message;
    }


#define MGE_ERROR_LOG(topic) \
    if(!mge::log::enabled(#topic, log_severity::ERROR_SEVERITY)) {} else mge::log_context(mge::log_instance_##topic(log_severity::ERROR_SEVERITY)).stream()

#define MGE_DEFINE_LOG(topic)                     \
    __declspec(dllexport)                         \
    log&                                          \
    log_instance_##topic(log_severity s)          \
    {                                             \
        static thread_local log instance(#topic); \
        return instance.set_severity(s);          \
    }
}
