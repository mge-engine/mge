// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/crash.hpp"
#include "mge/core/stacktrace.hpp"

#include <cstdlib>
#include <stdarg.h>

namespace mge {
    void crash()
    {
        crash("Crash!!!");
    }

    void crash(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        va_list args2;
        va_copy(args2, args);
        size_t needed = 1 + vsnprintf(nullptr, 0, fmt, args);
        char*  buf = (char*)alloca(needed);
        va_end(args);
        vsnprintf(buf, needed, fmt, args2);
        stacktrace st;
        fmt::print(stderr, "{}\n{}", buf, st);
        fflush(stdout);
        fflush(stderr);
        va_end(args);
        abort();
    }

} // namespace mge