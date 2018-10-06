// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/log_sink.hpp"
#include "mge/core/log_formatter.hpp"
#include <fstream>

namespace mge {
    class stdout_log_sink
            : public log_sink
    {
    public:
        stdout_log_sink()
            :log_sink(true)
            ,m_stream("log.txt")
        {
            m_formatter = log_formatter::create("text");
        }

        virtual ~stdout_log_sink() = default;

        void on_publish(const log_record& r) override
        {
            std::cout << "on publish" << std::endl;
            if(!m_formatter) {
                m_formatter = log_formatter::create("text");
            }
            if(m_formatter) {
                m_formatter->format(m_stream, r);
            }
        }
    private:
        log_formatter_ref m_formatter;
        std::ofstream m_stream;
    };
#ifdef stdout
#  undef stdout
#endif
    MGE_REGISTER_IMPLEMENTATION(stdout_log_sink, log_sink, stdout);
}
