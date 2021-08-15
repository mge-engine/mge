// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/input/dllexport.hpp"
#include <iosfwd>

namespace mge {

    /**
     * @brief A mouse (button) action.
     */
    enum class MGEINPUT_EXPORT mouse_action
    {
        PRESS = 0,
        RELEASE = 1,
#ifndef DOUBLE_CLICK
        DOUBLE_CLICK = 2,
#endif
        MOUSE_ACTION_DOUBLE_CLICK = 2
    };

    MGEINPUT_EXPORT std::ostream& operator<<(std::ostream&       os,
                                             const mouse_action& a);
} // namespace mge
