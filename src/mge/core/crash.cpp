// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/crash.hpp"
#include "mge/core/stacktrace.hpp"

#include <cstdlib>
#include <iostream>

namespace mge {
    void crash()
    {
        stacktrace st;
        std::cerr << "Crash" << std::endl << st << std::endl;
        abort();
    }
} // namespace mge