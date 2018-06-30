// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/executable_name.hpp"

#ifdef MGE_OS_WINDOWS
#  include <windows.h>
#endif

namespace mge {
    std::string executable_name()
    {
#if defined(MGE_OS_LINUX) || defined(MGE_OS_MACOSX)
            return std::string(__progname);
#else
            char buffer[2048];
            GetModuleFileName(0, buffer, sizeof(buffer));

            char *dot = strrchr(buffer, '.');
#if 0
            if(dot == nullptr) {
                throw MGE_EXCEPTION(illegal_state)
                   << "Cannot compute executable name (suffix not found).";
            }
#endif
            const char *basenamestart = strrchr(buffer, '\\');
#if 0
            if(basenamestart == nullptr) {
                throw MGE_EXCEPTION(illegal_state)
                   << "Cannot compute executable name (directory not found).";
            }
#endif
            return std::string((const char *)basenamestart+1,
                               (const char *)dot);
#endif
    }
}
