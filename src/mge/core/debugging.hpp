// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"

namespace mge {

    /**
     * @brief Raises a debug breakpoint.
     *
     * This method will call into the debugger.
     */
    void MGECORE_EXPORT breakpoint() noexcept;

    /**
     * @brief Checks if a debugger is present.
     *
     * @return @c true if a debugger is present, @c false otherwise.
     */
    bool MGECORE_EXPORT is_debugger_present() noexcept;

    /**
     * @brief Calls the debugger.
     *
     * This method will call into the debugger if a debugger is present.
     * If no debugger is present, a crash will be triggered.
     */
    void MGECORE_EXPORT breakpoint_if_debugging() noexcept;

} // namespace mge