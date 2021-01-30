// mge - Modern Game Engine
// Copyright (c) 2019 by Alexander Schroeder
// All rights reserved.
#include "mge/core/trace_level.hpp"
#include <iostream>
namespace mge {
    std::ostream &operator<<(std::ostream &os, const trace_level &l)
    {
        switch (l) {
        case trace_level::DEBUG:
            return os << "DEBUG";
        case trace_level::INFO:
            return os << "INFO";
        case trace_level::WARNING:
            return os << "WARNING";
        case trace_level::ERROR:
            return os << "ERROR";
        case trace_level::FATAL:
            return os << "FATAL";
        default:
            return os << "INVALID(" << static_cast<int>(l) << ")";
        }
    }
} // namespace mge