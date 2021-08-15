// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/trace_formatter.hpp"
#include "mge/core/trace_sink.hpp"

namespace mge {
    /**
     * @brief Stream trace sink that writes trace records to a stream.
     *
     */
    class MGECORE_EXPORT stream_trace_sink : public trace_sink
    {
    public:
        /**
         * @brief Construct a new stream trace sink object.
         * The target stream must have a life time longer than this sink.
         * @param stream    target stream
         * @param formatter formatter used to format record
         */
        stream_trace_sink(std::ostream&              stream,
                          const trace_formatter_ref& formatter);
        ~stream_trace_sink() = default;

        void publish(const trace_record& r) override;

    private:
        std::ostream*       m_stream;
        trace_formatter_ref m_formatter;
    };
} // namespace mge
