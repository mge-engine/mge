// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/crash.hpp"
#include <cstdlib>
#include <windows.h>

#include <iostream>

namespace mge {
    void crash()
    {
        exit(1);
    }


    void crash(const char *message)
    {
        std::cerr << message << std::endl;
        exit(1);
    }
}
