// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/system_error.hpp"

namespace mge {
    void
    system_error::clear()
    {
#ifdef MGE_OS_WINDOWS
        SetLastError(0);
#else
#  error Not implemented.
#endif
    }
}
