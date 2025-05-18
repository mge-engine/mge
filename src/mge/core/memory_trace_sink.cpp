// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/memory_trace_sink.hpp"
#include "mge/core/dllexport.hpp"

namespace mge {
    memory_trace_sink::memory_trace_sink(memory_trace_sink::capacity_type c)
    {
        set_capacity(c);
    }

    void memory_trace_sink::publish(const trace_record& r)
    {
        m_records.push_back(record(r));
    }

    memory_trace_sink::capacity_type memory_trace_sink::capacity() const
    {
        return m_records.capacity();
    }

    void memory_trace_sink::set_capacity(memory_trace_sink::capacity_type c)
    {
        m_records.set_capacity(c);
    }

    memory_trace_sink::size_type memory_trace_sink::size() const
    {
        return m_records.size();
    }

    void memory_trace_sink::clear()
    {
        m_records.clear();
    }

    void memory_trace_sink::forward(trace_sink& s)
    {
        for (const auto& r : m_records) {
            s.publish(r);
        }
    }

    memory_trace_sink::record::record(const trace_record& r)
        : trace_record(r)
        , materialized_message(r.message)
    {
        message = std::string_view(materialized_message);
    }

} // namespace mge