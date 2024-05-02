// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
/** @file */
#pragma once

#include "mge/config.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/enum.hpp"

#include <cstdint>
#include <iosfwd>

#ifdef MGE_OS_WINDOWS
#    if defined(ERROR)
#        undef ERROR
#    endif
#endif

namespace mge {
    /**
     * @brief Trace level of trace event.
     */
    enum class trace_level : uint8_t
    {
        NONE = 0,        //!< special level for enabling/disabling
        DEBUG = 1,       //!< debug message
        INFO = 2,        //!< information message
        WARNING = 4,     //!< warning message, potential problem
        ERROR = 8,       //!< severe malfunction message
        LEVEL_ERROR = 8, //!< alias for ERROR which is sometimes defined and
                         //!< can't be used as identifier
        FATAL = 16,      //!< fatal message, program will exit
        ALL = 31         //!< special level for enabling/disabling
    };
} // namespace mge