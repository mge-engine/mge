/*
 * Copyright (c) 2017 by Alexander Schroeder
 */
#pragma once

#include "mge/core/dllexport.hpp"
#include <iosfwd>

namespace mge {

    enum class MGE_CORE_EXPORT log_severity
    {
        NONE    =  0,
#ifndef ERROR
        ERROR   =  1,
#endif
        ERROR_SEVERITY = 1,
        WARNING =  2,
        WARNING_SEVERITY = 2,
        INFO    =  4,
        INFO_SEVERITY = 4,
#ifndef DEBUG
        DEBUG   =  8,
#endif
        DEBUG_SEVERITY = 8,
        ALL     = 15
    };

    MGE_CORE_EXPORT std::ostream& operator <<(std::ostream& os,
                                              const log_severity& severity);
}
