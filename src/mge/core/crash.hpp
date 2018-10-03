// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/stream_values.hpp"
#include <sstream>

namespace mge {
    /**
     * Crashes the program with a dump and without a message.
     */
    void MGE_CORE_EXPORT crash();

    /**
     * Crashes the program with a dump and a message.
     * @param message message displayed
     */
    void MGE_CORE_EXPORT crash(const char *message);

    /**
     * Formats message and crashes.
     * @param args values printed as message before crash.
     */
    template <typename... Args>
    inline void crash(Args ... args)
    {
        std::stringstream ss;
        stream_values(ss, args...);
        std::string message(ss.str());
        crash(message.c_str());
    }

}
