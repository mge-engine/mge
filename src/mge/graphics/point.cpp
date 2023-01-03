// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/point.hpp"
#include <iostream>

namespace mge {
    std::ostream& operator<<(std::ostream& os, const point& p)
    {
        return os << "point{x=" << p.x << ", y=" << p.y << "}";
    }
} // namespace mge
