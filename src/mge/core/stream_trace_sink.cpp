// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/stream_trace_sink.hpp"

namespace mge {
    stream_trace_sink::stream_trace_sink(std::ostream&              stream,
                                         const trace_formatter_ref& formatter)
        : m_stream(&stream)
        , m_formatter(formatter)
    {}

    void stream_trace_sink::publish(const trace_record& r)
    {
        m_formatter->format(*m_stream, r);
    }

} // namespace mge