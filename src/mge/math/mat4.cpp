// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/math/mat4.hpp"
#include <iostream>

namespace mge {

    std::ostream &operator<<(std::ostream &os, const fmat4 &m)
    {
        for (uint32_t row = 0; row < 4; ++row) {
            os << "|" << m[0][row] << " " << m[1][row] << " " << m[2][row]
               << " " << m[3][row] << "|\n";
        }
        return os;
    }

} // namespace mge