// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/trace_topic.hpp"

namespace mge {

    MGE_DEFINE_TRACE(MGE);
    MGE_DEFINE_TRACE(CORE);

    trace_topic::~trace_topic() {}

    bool trace_topic::enabled(trace_level l) const noexcept
    {
        uint8_t l_int = static_cast<uint8_t>(l);
        return (l_int & m_enabled_levels) != 0;
    }

    void trace_topic::enable(trace_level l)
    {
        uint8_t l_int = static_cast<uint8_t>(l);
        m_enabled_levels |= l_int;
    }

    void trace_topic::disable(trace_level l)
    {
        uint8_t l_int_inverted = ~static_cast<uint8_t>(l);
        m_enabled_levels &= l_int_inverted;
    }

    void trace_topic::set_level(trace_level l)
    {
        m_enabled_levels = static_cast<uint8_t>(l);
    }

    void trace_topic::set_level(uint8_t l)
    {
        uint8_t l_all    = static_cast<uint8_t>(trace_level::ALL);
        m_enabled_levels = l & l_all;
    }

    std::string_view trace_topic::name() const noexcept { return m_topic; }

    bool trace_topic::global() const noexcept
    {
        return this == &__trace_topic_MGE();
    }

    void trace_topic::publish(const trace_record &r)
    {
        for (auto &s : m_sinks) {
            s->publish(r);
        }

        for (auto &s : __trace_topic_MGE().m_sinks) {
            s->publish(r);
        }
    }

    void trace_topic::add_sink(const std::shared_ptr<trace_sink> &sink)
    {
        m_sinks.emplace_back(sink);
    }

    void trace_topic::remove_sink(const std::shared_ptr<trace_sink> &sink)
    {
        for (sink_vector::iterator i = m_sinks.begin(); i != m_sinks.end();
             ++i) {
            if (&(*sink) == &(*(*i))) {
                m_sinks.erase(i);
                break;
            }
        }
    }
} // namespace mge