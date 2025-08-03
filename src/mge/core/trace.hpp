// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/clock.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/format.hpp"
#include "mge/core/thread.hpp"
#include "mge/core/trace_level.hpp"
#include "mge/core/trace_topic.hpp"
#include <optional>
#include <source_location>
#include <sstream>

namespace mge {

    /**
     * @brief Trace object.
     * Objects of this class are temporarily constructed
     * to create trace records.
     */
    class MGECORE_EXPORT trace
    {
    public:
        /**
         * @brief Construct a new trace object.
         *
         * @param topic trace topic
         * @param level level of trace event
         * @param loc source location
         */
        trace(
            trace_topic&                topic,
            const trace_level           level,
            const std::source_location& loc = std::source_location::current());

        /**
         * Destructs trace object and before flushes the record,
         * if trace is enabled.
         */
        ~trace();

        /**
         * @brief Get the stream to write into.
         */
        std::ostream& stream() noexcept
        {
            if (!m_stream) {
                m_stream.emplace();
            }
            return *m_stream;
        }

        /**
         * @brief Write into trace. No-op if trace level is disabled.
         *
         * @tparam T type of written value
         * @param value value to write
         * @return @c *this
         */
        template <typename T> trace& operator<<(T&& value)
        {
            if (m_enabled) {
                (*m_stream) << value;
            }
            return *this;
        }

        /**
         * @brief Traces wide character string
         *
         * @param value string to trace
         * @return @c *this
         */
        trace& operator<<(const std::wstring& value);

    private:
        void flush();

        trace_topic& m_topic;
        bool         m_enabled;

        struct entry
        {
            clock::time_point      time;
            uint32_t               sequence;
            trace_level            level;
            mge::thread::system_id thread;
            const char*            file;
            const char*            function;
            uint32_t               line;
        };

        std::optional<entry>             m_entry;
        std::optional<std::stringstream> m_stream;
    };

/**
 * @def MGE_TRACE_OBJECT
 * @brief Invoke trace.
 * @param TOPIC trace topic
 * @param LEVEL trace level (only identifier within @c mge::trace_level scope)
 */
#define MGE_TRACE_OBJECT(TOPIC, LEVEL)                                         \
    ::mge::trace(MGE_TRACE_TOPIC(TOPIC), ::mge::trace_level::LEVEL)

/**
 * @def MGE_TRACE_ENABLED
 * @brief Check whether trace is enabled.
 * @param TOPIC trace topic
 * @param LEVEL trace level (only identifier within @c mge::trace_level scope)
 */
#define MGE_TRACE_ENABLED(TOPIC, LEVEL)                                        \
    MGE_TRACE_TOPIC(TOPIC).enabled(::mge::trace_level::LEVEL)

#define MGE_TRACE(TOPIC, LEVEL, ...)                                           \
    do {                                                                       \
        if (!(MGE_TRACE_ENABLED(TOPIC, LEVEL))) {                              \
        } else {                                                               \
            auto trc = MGE_TRACE_OBJECT(TOPIC, LEVEL);                         \
            fmt::print(trc.stream(), __VA_ARGS__);                             \
        }                                                                      \
    } while (false)

/**
 * @def MGE_DEBUG_TRACE_STREAM
 * @brief Invoke debug trace.
 * @param TOPIC trace topic
 */
#define MGE_DEBUG_TRACE_STREAM(TOPIC) MGE_TRACE_OBJECT(TOPIC, DEBUG)

/**
 * @def MGE_DEBUG_TRACE_ENABLED
 * @brief Check whether DEBUG trace is enabled.
 * @param TOPIC trace topic
 */
#define MGE_DEBUG_TRACE_ENABLED(TOPIC) MGE_TRACE_ENABLED(TOPIC, DEBUG)

/**
 * @def MGE_INFO_TRACE_ENABLED
 * @brief Check whether INFO trace is enabled.
 * @param TOPIC trace topic
 */
#define MGE_INFO_TRACE_ENABLED(TOPIC) MGE_TRACE_ENABLED(TOPIC, INFO)

/**
 * @def MGE_INFO_TRACE
 * @brief Invoke info trace.
 * @param TOPIC trace topic
 * @param ... format string and arguments
 */
#define MGE_INFO_TRACE(TOPIC, ...)                                             \
    do {                                                                       \
        if (!(MGE_INFO_TRACE_ENABLED(TOPIC))) {                                \
        } else {                                                               \
            auto trc = MGE_TRACE_OBJECT(TOPIC, INFO);                          \
            fmt::print(trc.stream(), __VA_ARGS__);                             \
        }                                                                      \
    } while (false)

/**
 * @def MGE_WARNING_TRACE_ENABLED
 * @brief Check whether WARNING trace is enabled.
 * @param TOPIC trace topic
 */
#define MGE_WARNING_TRACE_ENABLED(TOPIC) MGE_TRACE_ENABLED(TOPIC, WARNING)

/**
 * @def MGE_WARNING_TRACE
 * @brief Invoke warning trace.
 * @param TOPIC trace topic
 * @param ... format string and arguments
 */
#define MGE_WARNING_TRACE(TOPIC, ...)                                          \
    do {                                                                       \
        if (!(MGE_WARNING_TRACE_ENABLED(TOPIC))) {                             \
        } else {                                                               \
            auto trc = MGE_TRACE_OBJECT(TOPIC, WARNING);                       \
            fmt::print(trc.stream(), __VA_ARGS__);                             \
        }                                                                      \
    } while (false)

/**
 * @def MGE_ERROR_TRACE_ENABLED
 * @brief Check whether ERROR trace is enabled.
 * @param TOPIC trace topic
 */
#define MGE_ERROR_TRACE_ENABLED(TOPIC) MGE_TRACE_ENABLED(TOPIC, ERROR)

/**
 * @def MGE_ERROR_TRACE
 * @brief Invoke error trace.
 * @param TOPIC trace topic
 * @param ... format string and arguments
 */
#define MGE_ERROR_TRACE(TOPIC, ...)                                            \
    do {                                                                       \
        if (!(MGE_ERROR_TRACE_ENABLED(TOPIC))) {                               \
        } else {                                                               \
            auto trc = MGE_TRACE_OBJECT(TOPIC, ERROR);                         \
            fmt::print(trc.stream(), __VA_ARGS__);                             \
        }                                                                      \
    } while (false)

/**
 * @def MGE_FATAL_TRACE_ENABLED
 * @brief Check whether FATAL trace is enabled.
 * @param TOPIC trace topic
 */
#define MGE_FATAL_TRACE_ENABLED(TOPIC) MGE_TRACE_ENABLED(TOPIC, FATAL)

/**
 * @def MGE_FATAL_TRACE
 * @brief Invoke fatal trace.
 * @param TOPIC trace topic
 * @param ... format string and arguments
 */
#define MGE_FATAL_TRACE(TOPIC, ...)                                            \
    do {                                                                       \
        if (!(MGE_FATAL_TRACE_ENABLED(TOPIC))) {                               \
        } else {                                                               \
            auto trc = MGE_TRACE_OBJECT(TOPIC, FATAL);                         \
            fmt::print(trc.stream(), __VA_ARGS__);                             \
        }                                                                      \
    } while (false)

/**
 * @def MGE_XDEBUG
 * @brief Invoke debug trace.
 *
 * Used for temporary tracing when hunting bugs or alike. Submitted changes
 * should never have a MGE_XDEBUG trace statement.
 *
 * @param TOPIC trace topic
 */
#define MGE_XDEBUG(TOPIC) MGE_DEBUG_TRACE_STREAM(TOPIC) << "XDEBUG: "

} // namespace mge
