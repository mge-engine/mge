// mge - Modern Game Engine
// Copyright (c) 2019 by Alexander Schroeder
// All rights reserved.
#include "mge/core/dllexport.hpp"
#include "mge/core/trace_sink.hpp"
#include <boost/circular_buffer.hpp>

namespace mge {
    /**
     * @brief Memory trace sink.
     * A memory trace sink stores trace records in a circular buffer. It is used
     * to capture trace records which may get finally published using another
     * sink.
     */
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
        /**
         * @brief Type of this sink's capacity.
         */
        using capacity_type = record_buffer::capacity_type;
        /**
         * @brief Type of this sink's size.
         */
        using size_type = record_buffer::size_type;

        memory_trace_sink();
        ~memory_trace_sink();

        void publish(const trace_record &r) override;

        /**
         * @brief Capacity of this sink.
         *
         * @return current capacity of this sink, i.e. number of trace records
         * that can be stored
         */
        capacity_type capacity() const;
        /**
         * @brief Set the capacity of this sink.
         *
         * @param c new capacity
         */
        void set_capacity(capacity_type c);
        /**
         * @brief Size of this sink, i.e. number of stored records.
         *
         * @return number of stored records
         */
        size_type size() const;
        /**
         * @brief Clears the sink-
         *
         */
        void clear();

        /**
         * @brief Forwards all entries to another sink.
         *
         * @param sink target sink
         */
        void forward(trace_sink &sink);

    private:
        record_buffer m_records;
    };

} // namespace mge