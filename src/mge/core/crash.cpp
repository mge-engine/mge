// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/crash.hpp"
#include "mge/core/log.hpp"
#include <cstdlib>
#include <windows.h>

#include <iostream>

namespace mge {

    MGE_DEFINE_LOG(CRASH)

    void crash()
    {
        MGE_ERROR_LOG(CRASH)
                << "mge::crash called, program will exit"
                << std::endl;
        exit(1);
    }


    void crash(const char *message)
    {
        std::cerr << message << std::endl;
        exit(1);
    }

}
