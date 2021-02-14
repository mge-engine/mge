// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/dimension.hpp"

namespace mge {
    std::ostream &operator<<(std::ostream &os, const dimension &d)
    {
        return os << "dimension{width=" << d.width << ", height=" << d.height
                  << "}"
    }
} // namespace mge