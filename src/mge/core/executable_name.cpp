// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/executable_name.hpp"

#ifdef MGE_OS_WINDOWS
#    include <windows.h>
#endif

#ifdef MGE_OS_MACOSX
#    include <mach-o/dyld.h>
#endif

namespace mge {
    std::string executable_name()
    {
#ifdef MGE_OS_WINDOWS
        char buffer[2048];
        GetModuleFileName(0, buffer, sizeof(buffer));

        char*       dot = strrchr(buffer, '.');
        const char* basenamestart = strrchr(buffer, '\\');
        return std::string((const char*)basenamestart + 1, (const char*)dot);
#elif defined(MGE_OS_MACOSX)
        char     buffer[2048];
        uint32_t size = sizeof(buffer);
        _NSGetExecutablePath(buffer, &size);

        char*       dot = strrchr(buffer, '.');
        const char* basenamestart = strrchr(buffer, '/');
        return std::string((const char*)basenamestart + 1, (const char*)dot);
#else
#    error Missing port
#endif
    }

    std::string executable_path()
    {
#ifdef MGE_OS_WINDOWS
        char buffer[2048];
        GetModuleFileName(0, buffer, sizeof(buffer));

        char* basenamestart = strrchr(buffer, '\\');
        return std::string(buffer, basenamestart);
#elif defined(MGE_OS_MACOSX)
        char     buffer[2048];
        uint32_t size = sizeof(buffer);
        _NSGetExecutablePath(buffer, &size);

        char* basenamestart = strrchr(buffer, '/');
        return std::string(buffer, basenamestart);
#else
#    error Missing port
#endif
    }

} // namespace mge
