// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/log_sink.hpp"
#include "mge/core/log_formatter.hpp"
#include "mge/core/configuration.hpp"
#include <iostream>
#include <mutex>

namespace mge {

    class stdout_log_sink
            : public log_sink
    {
    public:
        stdout_log_sink()
            :log_sink(true)
        {
            m_formatter = log_formatter::create("text");
        }

        virtual ~stdout_log_sink() = default;

        void configure(const configuration& c) override
        {
            m_formatter = log_formatter::create(c.value("formatter"));
        }

        void on_publish(const log_record& r) override
        {
            if(!m_formatter) {
                return;
            }
            {
                std::lock_guard<decltype(m_output_lock)> guard(m_output_lock);
                if(m_formatter) {
                    m_formatter->format(std::cout, r);
                }
            }
        }
    private:
        log_formatter_ref m_formatter;
        std::mutex        m_output_lock;
    };
#ifdef stdout
#  undef stdout
#endif
    MGE_REGISTER_IMPLEMENTATION(stdout_log_sink, log_sink, stdout, console);
}
