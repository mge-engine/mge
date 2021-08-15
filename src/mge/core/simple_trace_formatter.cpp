// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/simple_trace_formatter.hpp"
#include "mge/core/trace_topic.hpp"
#include <cctype>
#include <iomanip>
#include <iostream>

using namespace std::chrono_literals;

namespace mge {
    simple_trace_formatter::simple_trace_formatter() {}

    void simple_trace_formatter::format(std::ostream&       stream,
                                        const trace_record& r)
    {
        if (!m_first.has_value()) {
            m_first = r.time;
        }

        auto duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
            r.time - *m_first);
        double duration_seconds =
            static_cast<double>(duration_ns.count()) / 1000000000.0;

        stream << "[" << std::fixed << std::setw(11) << std::setprecision(6)
               << std::setfill(' ') << duration_seconds << "] " << std::setw(10)
               << std::dec << r.sequence << " " << std::setw(8) << std::hex
               << r.thread << " ";
        switch (r.level) {
        case trace_level::DEBUG:
            stream << "D ";
            break;
        case trace_level::INFO:
            stream << "I ";
            break;
        case trace_level::WARNING:
            stream << "W ";
            break;
        case trace_level::ERROR:
            stream << "E ";
            break;
        case trace_level::FATAL:
            stream << "F ";
            break;
        default:
            stream << "  ";
            break;
        }
        stream << " " << std::setw(16) << r.topic->name();
        if (r.message.empty()) {
            stream << std::endl;
        } else {
            auto msg_begin = r.message.cbegin();
            auto msg_end = r.message.cend();
            while (msg_begin != msg_end) {
                auto c = *(msg_end - 1);
                if (c == ' ' || c == '\n' || c == '\r') {
                    --msg_end;
                } else {
                    stream << " " << std::string_view(msg_begin, msg_end)
                           << std::endl;
                    break;
                }
            }
        }
    } // namespace mge
} // namespace mge
