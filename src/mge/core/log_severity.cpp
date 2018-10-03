/*
 * Copyright (c) 2017 by Alexander Schroeder
 */
#include "mge/core/log_severity.hpp"
#include <iostream>

namespace mge {
    std::ostream& operator <<(std::ostream& os, const log_severity& severity)
    {
        switch(severity) {
        case log_severity::NONE:
            os << "NONE";
            break;
        case log_severity::ERROR:
            os << "ERROR";
            break;
        case log_severity::WARNING:
            os << "WARNING";
            break;
        case log_severity::INFO:
            os << "INFO";
            break;
        case log_severity::DEBUG:
            os << "DEBUG";
            break;
        case log_severity::ALL:
            os << "ALL";
            break;
        }
        return os;
    }
}
