// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/core/dllexport.hpp"

namespace mge {

    /**
     * @brief Terminates the application, writing a crash dump.
     */
    MGE_NO_RETURN void MGECORE_EXPORT crash();

    /**
     * @brief Terminates the application, printing a specific message.
     *
     * @param fmt format string
     * @param ... format arguments
     */
    MGE_NO_RETURN void MGECORE_EXPORT crash(const char* fmt, ...);

} // namespace mge