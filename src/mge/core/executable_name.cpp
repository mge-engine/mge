// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/executable_name.hpp"

#ifdef MGE_OS_WINDOWS
#    include <windows.h>
#elif defined(MGE_OS_LINUX)
#    include <cstring>
#    include <unistd.h>
#elif defined(MGE_OS_MACOSX)
#    include <mach-o/dyld.h>
#else
#    error Missing port
#endif

namespace mge {
    std::pmr::string
    executable_name(std::pmr::memory_resource* resource)
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
        return std::pmr::string(
            basenamestart + 1,
            static_cast<size_t>(dot - basenamestart - 1),
            resource);
#elif defined(MGE_OS_LINUX)
        char    buffer[2048];
        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
        if (len == -1) {
            len = 0;
        }
        buffer[len] = '\0';

        char* slash = strrchr(buffer, '/');
        if (slash == nullptr) {
            return std::pmr::string(buffer, resource);
        } else {
            return std::pmr::string(slash + 1, resource);
        }
#elif defined(MGE_OS_MACOSX)
        char     buffer[2048];
        uint32_t size = sizeof(buffer);
        if (_NSGetExecutablePath(buffer, &size) != 0) {
            return std::pmr::string(resource);
        }
        char* slash = strrchr(buffer, '/');
        if (slash == nullptr) {
            return std::pmr::string(buffer, resource);
        } else {
            return std::pmr::string(slash + 1, resource);
        }
#else
#    error Missing port
#endif
    }

    std::pmr::string
    executable_path(std::pmr::memory_resource* resource)
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
        return std::pmr::string(
            buffer,
            static_cast<size_t>(basenamestart - buffer),
            resource);
#elif defined(MGE_OS_LINUX)
        char    buffer[2048];
        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
        if (len == -1) {
            len = 0;
        }
        buffer[len] = '\0';
        char* slash = strrchr(buffer, '/');
        if (slash == nullptr) {
            return std::pmr::string(resource);
        } else {
            return std::pmr::string(
                buffer,
                static_cast<size_t>(slash - buffer),
                resource);
        }
#elif defined(MGE_OS_MACOSX)
        char     buffer[2048];
        uint32_t size = sizeof(buffer);
        if (_NSGetExecutablePath(buffer, &size) != 0) {
            return std::pmr::string(resource);
        }
        char* slash = strrchr(buffer, '/');
        if (slash == nullptr) {
            return std::pmr::string(resource);
        } else {
            return std::pmr::string(
                buffer,
                static_cast<size_t>(slash - buffer),
                resource);
        }
#else
#    error Missing port
#endif
    }

} // namespace mge
