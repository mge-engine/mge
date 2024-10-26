// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/executable_name.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/system_error.hpp"

#ifdef MGE_OS_WINDOWS
#    include <windows.h>
#elif MGE_OS_LINUX
#    include <cstring>
#    include <limits.h>
#    include <unistd.h>
#endif

namespace mge {
    std::string executable_name()
    {
#if MGE_OS_WINDOWS
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
#elif MGE_OS_LINUX
        char    buffer[PATH_MAX];
        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
        if (len == -1) {
            MGE_CHECK_SYSTEM_ERROR(readlink);
        }
        buffer[len] = '\0';

        const char* basenamestart = strrchr(buffer, '/');
        if (basenamestart == nullptr) {
            MGE_THROW(illegal_state)
                << "Cannot compute executable name (directory not found).";
        }
        return std::string((const char*)basenamestart + 1);
#else
#    error Missing port
#endif
    }

    std::string executable_path()
    {
#if MGE_OS_WINDOWS
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
#elif MGE_OS_LINUX

#else
#    error Missing port
#endif
    }

} // namespace mge
