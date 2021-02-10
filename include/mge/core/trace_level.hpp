// mge - Modern Game Engine
// Copyright (c) 2019 by Alexander Schroeder
// All rights reserved.
/** @file */
#pragma once

#include "mge/core/dllexport.hpp"
#include <cstdint>
#include <iosfwd>

namespace mge {
    /**
     * @brief Trace level of trace event.
     */
    enum class trace_level : uint8_t
    {
        NONE    = 0,  //!< special level for enabling/disabling
        DEBUG   = 1,  //!< debug message
        INFO    = 2,  //!< information message
        WARNING = 4,  //!< warning message, potential problem
        ERROR   = 8,  //!< severe malfunction message
        FATAL   = 16, //!< fatal message, program will exit
        ALL     = 31  //!< special level for enabling/disabling
    };

    MGECORE_EXPORT std::ostream &operator<<(std::ostream &     os,
                                            const trace_level &l);
} // namespace mge