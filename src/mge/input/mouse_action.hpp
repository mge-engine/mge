// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/input/dllexport.hpp"
#include <iosfwd>

namespace mge {

    enum class MGE_INPUT_EXPORT mouse_action
    {
        PRESS   = 0,
        RELEASE = 1,
#ifndef DOUBLE_CLICK
        DOUBLE_CLICK = 2,
#endif
        MOUSE_ACTION_DOUBLE_CLICK = 2
    };

    MGE_INPUT_EXPORT std::ostream &operator<<(std::ostream &      os,
                                              const mouse_action &a);
} // namespace mge
