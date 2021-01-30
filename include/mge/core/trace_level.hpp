// mge - Modern Game Engine
// Copyright (c) 2019 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/core/dllexport.hpp"
#include <cstdint>
#include <iosfwd>

namespace mge {
    enum class trace_level : uint8_t
    {
        DEBUG   = 1,
        INFO    = 2,
        WARNING = 4,
        ERROR   = 8,
        FATAL   = 16,
        ALL     = 31
    };

    MGECORE_EXPORT std::ostream &operator<<(std::ostream &     os,
                                            const trace_level &l);
} // namespace mge