// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/trace.hpp"
#include <atomic>

namespace mge {

    static std::atomic<uint32_t> s_trace_sequence;

    trace::trace(trace_topic &topic, const trace_level level)
        : m_topic(topic), m_enabled(topic.enabled(level))
    {
        if (m_enabled) {
            m_entry           = entry();
            m_entry->time     = clock::now();
            m_entry->level    = level;
            m_entry->sequence = ++s_trace_sequence;

            m_stream = std::stringstream();
        }
    }

    trace::~trace()
    {
        if (m_enabled) {
            flush();
        }
    }

    void trace::flush()
    {
        trace_record r;

        std::string msg = m_stream->str();

        r.level    = m_entry->level;
        r.sequence = m_entry->sequence;
        r.time     = m_entry->time;
        r.topic    = &m_topic;
        r.message  = std::string_view(msg.begin(), msg.end());
        m_topic.publish(r);
    }

} // namespace mge