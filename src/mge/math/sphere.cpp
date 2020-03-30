// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/math/sphere.hpp"
#include <iostream>
namespace mge {
    std::ostream &operator<<(std::ostream &os, const sphere &s)
    {
        return os << "sphere[center=" << s.center() << ", radius=" << s.radius()
                  << "]";
    }
} // namespace mge
