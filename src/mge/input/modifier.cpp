// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/input/modifier.hpp"
#include <iostream>

namespace mge {
    std::ostream& operator<<(std::ostream& os, const modifier_value& v)
    {
        switch (v) {
        case modifier_value::ALT:
            return os << "ALT";
        case modifier_value::SHIFT:
            return os << "SHIFT";
        case modifier_value::CONTROL:
            return os << "CONTROL";
        default:
            return os << "INVALID(" << (int)v << ")";
        }
    }
} // namespace mge
