// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/math/vec2.hpp"
#include <iostream>

namespace mge {

    std::ostream& operator<<(std::ostream& os, const fvec2& v) { return os << "(" << v.x << ", " << v.y << ")"; }

    std::ostream& operator<<(std::ostream& os, const ivec2& v) { return os << "(" << v.x << ", " << v.y << ")"; }

} // namespace mge