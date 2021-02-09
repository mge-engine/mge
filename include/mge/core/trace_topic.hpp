// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
/** @file */
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/trace_level.hpp"
#include "mge/core/trace_record.hpp"
#include "mge/core/trace_sink.hpp"

#include <memory>
#include <vector>

namespace mge {
    /**
     * @brief Trace topic.
     *
     * Trace can be enabled/disabled per level and topic.
     */
    class MGECORE_EXPORT trace_topic
    {
    public:
        /**
         * @brief Constructor.
         *
         * @tparam N length of string
         */
        template <std::size_t N>
        constexpr trace_topic(const char (&name)[N])
            : m_topic(static_cast<const char *>(name),
                      static_cast<const char *>(name) + N - 1),
              m_enabled_levels(0)
        {}

        ~trace_topic();

        /**
         * @brief Return whether a trace level is enabled
         *
         * @param l trace level
         * @return @c true if enabled
         * @return @c false if not enabled
         */
        bool enabled(trace_level l) const noexcept;

        /**
         * @brief Enable trace level.
         *
         * @param l enabled trace level
         */
        void enable(trace_level l);

        /**
         * @brief Disable trace level
         *
         * @param l disabled trace level
         */
        void disable(trace_level l);

        /**
         * @brief Set trace level.
         *
         * @param l trace level
         */
        void set_level(trace_level l);

        /**
         * @brief Set trace level.
         *
         * @param l trace level, or combination of levels
         */
        void set_level(uint8_t l);

        /**
         * @brief Name of trace topic.
         *
         * @return topic name
         */
        std::string_view name() const noexcept;

        /**
         * @brief Return whether topic is the global topic.
         *
         * @return @c true if it is the global topic
         * @return @c false if it is any other topic
         */
        bool global() const noexcept;

        /**
         * @brief Publish a trace record.
         *
         * @param r record
         */
        void publish(const trace_record &r);

        /**
         * @brief Add a trace sink.
         *
         * @param sink trace sink to add
         */
        void add_sink(const std::shared_ptr<trace_sink> &sink);

        /**
         * @brief Remove a sink.
         *
         * @param sink sink to remove
         */
        void remove_sink(const std::shared_ptr<trace_sink> &sink);

    private:
        using sink_vector = std::vector<std::shared_ptr<trace_sink>>;
        std::string_view m_topic;
        uint8_t          m_enabled_levels;
        sink_vector      m_sinks;
    };

/**
 * @def MGE_USE_TRACE
 * @brief Use an already defined trace topic locally.
 * @param TOPIC name of trace topic
 */
#define MGE_USE_TRACE(TOPIC) ::mge::trace_topic &__trace_topic_##TOPIC()

#define MGE_USE_IMPORTED_TRACE(TOPIC)                                          \
    MGE_DLLIMPORT ::mge::trace_topic &__trace_topic_##TOPIC()

#define MGE_DEFINE_TRACE(TOPIC)                                                \
    static mge::trace_topic s__trace_topic##TOPIC(#TOPIC);                     \
    MGE_DLLEXPORT mge::trace_topic &__trace_topic_##TOPIC()                    \
    {                                                                          \
        return s__trace_topic##TOPIC;                                          \
    }                                                                          \
    MGE_DLLEXPORT mge::trace_topic &__trace_topic_##TOPIC()

#define MGE_TRACE_TOPIC(TOPIC) __trace_topic_##TOPIC()

#define MGE_NS_TRACE_TOPIC(NS, TOPIC) NS::__trace_topic_##TOPIC()

} // namespace mge