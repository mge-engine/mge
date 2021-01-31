// mge - Modern Game Engine
// Copyright (c) 2019 by Alexander Schroeder
// All rights reserved.
#include "mge/core/trace.hpp"
#include <atomic>

namespace mge {

    static std::atomic<uint32_t> s_trace_sequence;

    trace::trace(const trace_topic &topic, const trace_level level)
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

    void trace::flush() {}

} // namespace mge