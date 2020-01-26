// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/log_record.hpp"
#include "mge/core/log_severity.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/streamutils.hpp"

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
        ~log() = default;

        log& begin_entry(log_severity s);
        bool enabled(log_severity s);
    private:
        friend class log_context;

        void flush_current_record();
        void publish(const log_record& r);

        log_record        m_log_record;
        std::stringstream m_message;
        uint8_t           m_severity_mask;
        uint32_t          m_config_version;
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

/**
 * @brief Write to error log.
 * @param topic log topic
 */
#define MGE_ERROR_LOG(topic)                                                    \
    if(!::log_instance_##topic().enabled(mge::log_severity::ERROR_SEVERITY))      \
    {}                                                                          \
    else                                                                        \
    mge::log_context(::log_instance_##topic().begin_entry(mge::log_severity::ERROR_SEVERITY)).stream()


/**
 * @brief Write to warning log.
 * @param topic log topic
 */
#define MGE_WARNING_LOG(topic)                                                    \
    if(!::log_instance_##topic().enabled(mge::log_severity::WARNING_SEVERITY))      \
    {}                                                                          \
    else                                                                        \
    mge::log_context(::log_instance_##topic().begin_entry(mge::log_severity::WARNING_SEVERITY)).stream()

/**
 * @brief Write to debug log.
 * @param topic log topic
 */
#define MGE_DEBUG_LOG(topic)                                                    \
    if(!::log_instance_##topic().enabled(mge::log_severity::DEBUG_SEVERITY))      \
    {}                                                                          \
    else                                                                        \
    mge::log_context(::log_instance_##topic().begin_entry(mge::log_severity::DEBUG_SEVERITY)).stream()

/**
 * @brief Write to info log.
 * @param topic log topic
 */
#define MGE_INFO_LOG(topic)                                                    \
    if(!::log_instance_##topic().enabled(mge::log_severity::INFO_SEVERITY))      \
    {}                                                                          \
    else                                                                        \
    mge::log_context(::log_instance_##topic().begin_entry(mge::log_severity::INFO_SEVERITY)).stream()

/**
 * @brief Defines a log instance for a topic.
 * @param topic log topic name
 */
#define MGE_DEFINE_LOG(topic)                          \
    __declspec(dllexport)                              \
    mge::log&                                          \
    log_instance_##topic()                             \
    {                                                  \
        static thread_local mge::log instance(#topic); \
        return instance;                               \
    }

/**
 * @brief Declare usage of a log.
 * @param topic log topic name
 */
#define MGE_USE_LOG(topic)                        \
    extern mge::log& log_instance_##topic()

/**
 * @brief Declare usage of a log imported from a different shared library.
 * @param topic log topic name
 */
#define MGE_USE_IMPORTED_LOG(topic)               \
    __declspec(dllimport)                         \
    mge::log& log_instance_##topic()

}
