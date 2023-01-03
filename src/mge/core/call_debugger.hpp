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
    void MGECORE_EXPORT call_debugger();

}