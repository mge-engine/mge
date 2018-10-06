// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/log_formatter.hpp"

namespace mge {

    static inline const char *
    severity_string(log_severity s)
    {
        switch(s) {
        case log_severity::DEBUG_SEVERITY:
            return "D";
        case log_severity::INFO_SEVERITY:
            return "I";
        case log_severity::ERROR_SEVERITY:
            return "E";
        case log_severity::WARNING_SEVERITY:
            return "W";
        default:
            return " ";
        }
    }

    class text_log_formatter : public log_formatter
    {
    public:
        text_log_formatter() = default;
        virtual ~text_log_formatter() = default;

        virtual void format(std::ostream& os, const log_record& r)
        {
            size_t message_len = strlen(r.message);
            const char *ep = r.message + message_len;
            while(*ep == '\n') {
                --ep;
                if(ep == r.message) {
                    break;
                }
            }
            auto t = std::chrono::system_clock::to_time_t(r.timestamp);
            auto subseconds = r.timestamp - std::chrono::system_clock::from_time_t(t);
            auto int_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    subseconds);
            auto lt = std::localtime(&t);
            os << std::put_time(lt, "%Y-%m-%d %H:%M:%S.");
            os << std::setw(9) << std::setfill('0') << int_ms.count() << std::setw(0)
               << std::setfill(' ') << " ";
            os << severity_string(r.severity) << ' ';
            os << std::setw(10) << std::left << std::setfill('0') << r.thread_id
               << std::setfill(' ') << std::internal << " ";

            os << std::setw(13) << r.category << " ";

            os.write(r.message, ep - r.message);
            os << std::endl;
        }
    };

    MGE_REGISTER_IMPLEMENTATION(text_log_formatter, log_formatter, text);
}
