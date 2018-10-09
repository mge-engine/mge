// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/system_error.hpp"
#include "mge/core/stdexceptions.hpp"

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

    system_error::error::error()
    {
#ifdef MGE_OS_WINDOWS
        value = GetLastError();
#else
#  error not implemented
#endif
    }

    MGE_DEFINE_EXCEPTION(system_error)
}
