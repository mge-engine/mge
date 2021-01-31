// mge - Modern Game Engine
// Copyright (c) 2019 by Alexander Schroeder
// All rights reserved.
#include "mge/core/dllexport.hpp"
#include "mge/core/trace_sink.hpp"
#include <boost/circular_buffer.hpp>

namespace mge {
    class MGECORE_EXPORT memory_trace_sink : public trace_sink
    {
    private:
        struct record : trace_record
        {
            record() = default;
            record(const trace_record &r);
            record(const record &) = default;
            record(record &&)      = default;

            record &    operator=(record &&) = default;
            record &    operator=(const record &) = default;
            std::string materialized_message;
        };

        using record_buffer = boost::circular_buffer<record>;

    public:
        using capacity_type = record_buffer::capacity_type;
        using size_type     = record_buffer::size_type;
        memory_trace_sink();
        ~memory_trace_sink();

        void publish(const trace_record &r) override;

        capacity_type capacity() const;
        void          set_capacity(capacity_type c);
        size_type     size() const;
        void          clear();

        void forward(trace_sink &sink);

    private:
        record_buffer m_records;
    };

} // namespace mge