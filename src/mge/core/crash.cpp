// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/crash.hpp"
#include "mge/core/stacktrace.hpp"
#include "mge/core/log.hpp"
#include <cstdlib>
#include <windows.h>

#include <iostream>

MGE_DEFINE_LOG(CRASH)

namespace mge {

    void crash()
    {
        MGE_ERROR_LOG(CRASH)
                << "crash";
        MGE_ERROR_LOG(CRASH) << mge::stacktrace();
        exit(1);
    }


    void crash(const char *message)
    {
        MGE_ERROR_LOG(CRASH)
                << "crash: " << message;
        MGE_ERROR_LOG(CRASH) << mge::stacktrace();
        exit(1);
    }

}
