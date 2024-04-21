// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/texture_type.hpp"
#include <iostream>

namespace mge {
    std::ostream& operator<<(std::ostream& os, const texture_type& t)
    {
        return os << std::format("{}", t);
    }
} // namespace mge
