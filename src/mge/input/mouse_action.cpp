// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/input/mouse_action.hpp"
#include <iostream>

namespace mge {
    std::ostream& operator<<(std::ostream& os, const mouse_action& a)
    {
        switch (a) {
        case mouse_action::PRESS:
            return os << "PRESS";
        case mouse_action::RELEASE:
            return os << "RELEASE";
        case mouse_action::MOUSE_ACTION_DOUBLE_CLICK:
            return os << "DOUBLE_CLICK";
        default:
            return os << "INVALID(" << (int)a << ")";
        }
    }
} // namespace mge
