// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/config.hpp"
#include "mge/core/format_string.hpp"
#include <sstream>

namespace mge {
    /**
     * Crashes the program with a dump and without a message.
     */
    MGE_NORETURN void MGE_CORE_EXPORT crash();

    /**
     * Crashes the program with a dump and a message.
     * @param message message displayed
     */
    MGE_NORETURN void MGE_CORE_EXPORT crash(const char *message);

    /**
     * Formats message and crashes.
     * @param args values printed as message before crash.
     */
    template <typename... Args> MGE_NORETURN inline void crash(Args... args)
    {
        crash(format_string(args...).c_str());
    }

} // namespace mge

/**
 * @define MGE_CRASH_ASSERT
 * @brief Crashes if the condition is evaluated to false.
 * @param cond condition to check
 * @param ... further elements of crash message
 */
#define MGE_CRASH_ASSERT(cond, ...)                                            \
    if (!(cond))                                                               \
    ::mge::crash("Assertion failed: ", #cond, ": ", __VA_ARGS__)
