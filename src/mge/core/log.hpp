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

    /**
     * Log (or logger).
     *
     * A log object is used to submit messages related to
     * a specific topic or component.
     *
     * Log objects are not hierarchical, i.e. there exists a
     * number of log objects that has no relation to each
     * other.
     *
     * A logger has a minimum severity configured, log messages
     * below that severity are ignored.
     */
    class MGE_CORE_EXPORT log
            : public noncopyable
    {
    public:
        /**
         * Constructor. Note logs are usually created and
         * accessed by using the @c MGE_DEFINE_LOG macro.
         * @param topic topic that is handled by the log
         */
        log(const char *topic);

        /**
         * Destructor.
         */
        ~log();

        log& begin_entry(log_severity s);
        bool enabled(log_severity s);
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


#define MGE_ERROR_LOG(topic)                                                    \
    if(!mge::log_instance_##topic().enabled(log_severity::ERROR_SEVERITY))      \
    {}                                                                          \
    else                                                                        \
    mge::log_context(mge::log_instance_##topic().begin_entry(log_severity::ERROR_SEVERITY)).stream()

#define MGE_DEFINE_LOG(topic)                     \
    __declspec(dllexport)                         \
    log&                                          \
    log_instance_##topic()                        \
    {                                             \
        static thread_local log instance(#topic); \
        return instance;                          \
    }

#define MGE_USE_LOG(topic)                        \
    extern log& log_instance_##topic()

#define MGE_USE_IMPORTED_LOG(topic)               \
    __declspec(dllimport)                         \
    log& log_instance_##topic()

}
