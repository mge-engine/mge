// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/core/dllexport.hpp"
#include <iosfwd>

namespace mge {

#ifdef MGE_DOXYGEN
    /**
     * Log message severity. Note the shortcuts @c log_severity::ERROR
     * and @c log_severity::DEBUG are only defined if the environment
     * does not define an @c ERROR or @c DEBUG macro.
     */
    enum class log_severity
    {
        NONE = 0,               //!< No severity specified.
        ERROR = 1,              //!< Error log severity, log message describes malfunction in program.
        ERROR_SEVERITY = 1,     //!< Error log severity, log message describes malfunction in program.
        WARNING = 2,            //!< Warning severity, log message describes a potential problem.
        WARNING_SEVERITY = 2,   //!< Warning severity, log message describes a potential problem.
        INFO = 3,               //!< Information message severity.
        INFO_SEVERITY = 3,      //!< Information message severity.
        DEBUG = 4,              //!< Debug message severity.
        DEBUG_SEVERITY = 4,     //!< Debug message severity.
        ALL = 15                //!< Pseudo severity that includes all other severities.
    };
#else
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
#endif
    /**
     * Print severity as text to output stream. Prints the short names (e.g.
     * @c DEBUG for @c DEBUG_SEVERITY).
     * @param os        output stream
     * @param severity  severity
     * @return @c os.
     */
    MGE_CORE_EXPORT std::ostream& operator <<(std::ostream& os,
                                              const log_severity& severity);
}
