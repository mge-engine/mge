// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/log_sink.hpp"
#include "mge/core/log_formatter.hpp"
#include "mge/core/file.hpp"
#include "mge/core/configuration.hpp"
#include <iostream>
#include <mutex>

namespace mge {

    class file_log_sink
            : public log_sink
    {
    public:
        file_log_sink()
            : log_sink(true)
            , m_configured(false)
        {
            m_formatter = log_formatter::create("text");
        }

        virtual ~file_log_sink() = default;

        void configure(const configuration& c) override
        {
            m_formatter = log_formatter::create(c.value("formatter"));
            m_file = std::make_shared<mge::file>(c.value("logfile_name"));
            m_stream = m_file->open_for_output();
            m_configured = true;
        }

        void on_publish(const log_record& r) override
        {
            if (!m_configured) {
                return;
            }
            {
                std::lock_guard<decltype(m_output_lock)> guard(m_output_lock);
                if (m_formatter) {
                    m_formatter->format(stream(), r);
                }
            }
        }
    private:
        std::ostream& stream()
        {
            return m_stream->ostream();
        }

        mge::file_ref          m_file;
        mge::output_stream_ref m_stream;
        log_formatter_ref      m_formatter;
        std::mutex             m_output_lock;
        bool                   m_configured;
    };

    MGE_REGISTER_IMPLEMENTATION(file_log_sink, log_sink, file);
}
