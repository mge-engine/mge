// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/executable_name.hpp"

#ifdef MGE_OS_WINDOWS
#    include <windows.h>
#elif defined(MGE_OS_LINUX)
#    include <unistd.h>
#    include <cstring>
#else
#    error Missing port
#endif

namespace mge {
    std::string executable_name()
    {
#ifdef MGE_OS_WINDOWS
        char buffer[2048];
        GetModuleFileName(0, buffer, sizeof(buffer));

        char* dot = strrchr(buffer, '.');
#if 0
        if (dot == nullptr) {
            throw MGE_EXCEPTION(illegal_state)
                << "Cannot compute executable name (suffix not found).";
        }
#endif
        const char* basenamestart = strrchr(buffer, '\\');
#if 0
            if(basenamestart == nullptr) {
                throw MGE_EXCEPTION(illegal_state)
                   << "Cannot compute executable name (directory not found).";
            }
#endif
        return std::string((const char*)basenamestart + 1, (const char*)dot);
#elif defined(MGE_OS_LINUX)
        char buffer[2048];
        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
        if (len == -1) {
            len = 0;
        }
        buffer[len] = '\0';

        char* slash = strrchr(buffer, '/');
        if (slash == nullptr) {
            return std::string(buffer);
        } else {
            return std::string(slash + 1);
        }
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
#if 0
        if (basenamestart == nullptr) {
            throw MGE_EXCEPTION(illegal_state)
                << "Cannot compute executable path (directory not found).";
        }
#endif
        return std::string(buffer, basenamestart);
#elif defined(MGE_OS_LINUX)
        char buffer[2048];
        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
        // TODO: error handling
        if (len == -1) {
            len = 0;
            buffer[0] = '\0';
        }
        char* slash = strrchr(buffer, '/');
        if (slash == nullptr) {
            return "";
        } else {
            return std::string(buffer, slash);
        }
#else
#    error Missing port
#endif
    }

} // namespace mge
