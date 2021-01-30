// mge - Modern Game Engine
// Copyright (c) 2019 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/trace_level.hpp"

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
         * @tparam N
         */
        template <std::size_t N>
        constexpr trace_topic(const char (&name)[N])
            : m_topic(static_cast<const char *>(name)), m_enabled_levels(0)
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
         * @param l trace level, or combination of levels
         */
        void set_level(uint8_t l);

        /**
         * @brief Name of trace topic.
         *
         * @return topic name
         */
        const char *name() const noexcept;

        /**
         * @brief Return whether topic is the global topic.
         *
         * @return @c true if it is the global topic
         * @return @c false if it is any other topic
         */
        bool global() const noexcept;

    private:
        const char *m_topic;
        uint8_t     m_enabled_levels;
    };

#define MGE_USE_TRACE(TOPIC)                                                   \
    extern ::mge::trace_topic __trace_topic_##TOPIC(#TOPIC)
#define MGE_USE_IMPORTED_TRACE(TOPIC)                                          \
    extern MGE_DLLIMPORT ::mge::trace_topic __trace_topic_##TOPIC

#define MGE_DEFINE_TRACE(TOPIC)                                                \
    mge::trace_topic MGE_DLLEXPORT __trace_topic_##TOPIC(#TOPIC)

#define MGE_TRACE_TOPIC(TOPIC) __trace_topic_##TOPIC

#define MGE_NS_TRACE_TOPIC(NS, TOPIC) NS::__trace_topic_##TOPIC

} // namespace mge