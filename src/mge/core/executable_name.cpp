// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/executable_name.hpp"

#ifdef MGE_OS_WINDOWS
#    include <windows.h>
#endif

#if defined(MGE_OS_MACOSX) || defined(MGE_OS_LINUX)
#    include <unistd.h>
#endif

#ifdef MGE_OS_MACOSX
#    include <libproc.h>
#endif

namespace mge {
    std::string executable_name()
    {
#ifdef MGE_OS_WINDOWS
        char buffer[2048];
        GetModuleFileName(0, buffer, sizeof(buffer));

        char* dot = strrchr(buffer, '.');
#    if 0
        if (dot == nullptr) {
            throw MGE_EXCEPTION(illegal_state)
                << "Cannot compute executable name (suffix not found).";
        }
#    endif
        const char* basenamestart = strrchr(buffer, '\\');
#    if 0
            if(basenamestart == nullptr) {
                throw MGE_EXCEPTION(illegal_state)
                   << "Cannot compute executable name (directory not found).";
            }
#    endif
        return std::string((const char*)basenamestart + 1, (const char*)dot);
#elif defined(MGE_OS_MACOSX) || defined(MGE_OS_LINUX)
        return std::string(getprogname());
#endif
    }

    std::string executable_path()
    {
#ifdef MGE_OS_WINDOWS
        char buffer[2048];
        GetModuleFileName(0, buffer, sizeof(buffer));

        char* basenamestart = strrchr(buffer, '\\');
#    if 0
        if (basenamestart == nullptr) {
            throw MGE_EXCEPTION(illegal_state)
                << "Cannot compute executable path (directory not found).";
        }
#    endif
        return std::string(buffer, basenamestart);
#elif defined(MGE_OS_MACOSX)
        char pathbuf[PROC_PIDPATHINFO_MAXSIZE];
        pathbuf[0] = '\0';
        proc_pidpath(getpid(), pathbuf, sizeof(pathbuf));
        return std::string(pathbuf);
#else
#    error Missing port
#endif
    }

} // namespace mge
