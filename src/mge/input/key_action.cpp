// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/input/key_action.hpp"
#include <iostream>

namespace mge {

    std::ostream& operator<<(std::ostream& os, const key_action& a)
    {
        switch (a) {
        case key_action::PRESS:
            return os << "PRESS";
        case key_action::RELEASE:
            return os << "RELEASE";
        case key_action::REPEAT:
            return os << "REPEAT";
        default:
            return os << "INVALID(" << (int)a << ")";
        }
    }
} // namespace mge
