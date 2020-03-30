// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/texture_type.hpp"
#include <iostream>

namespace mge {
    std::ostream &operator<<(std::ostream &os, const texture_type &t)
    {
        switch (t) {
#define CASE_OUTPUT(x)                                                         \
    case texture_type::x:                                                      \
        return os << #x
            CASE_OUTPUT(TEXTURE_1D);
            CASE_OUTPUT(TEXTURE_2D);
            CASE_OUTPUT(TEXTURE_3D);
            CASE_OUTPUT(TEXTURE_CUBE_MAP);
#undef CASE_OUTPUT
        default:
            return os << "INVALID(" << (int)t << ")";
        }
    }
} // namespace mge
