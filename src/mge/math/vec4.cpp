// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/math/vec4.hpp"
#include <iostream>

namespace mge {
    std::ostream& operator<<(std::ostream& os, const fvec4& v)
    {
        return os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w
                  << ")";
    }
} // namespace mge