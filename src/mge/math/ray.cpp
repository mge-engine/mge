// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/math/ray.hpp"
#include <iostream>

namespace mge {
    std::ostream &operator<<(std::ostream &os, const ray &r)
    {
        return os << "ray[origin=" << r.origin()
                  << ", direction=" << r.direction() << "]";
    }
} // namespace mge
