// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/math/vec3.hpp"
#include <iostream>

namespace mge {
    std::ostream& operator<<(std::ostream& os, const fvec3& v) { return os << "(" << v.x << ", " << v.y << ", " << v.z << ")"; }
} // namespace mge