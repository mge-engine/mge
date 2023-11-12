// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/shader_type.hpp"
#include <iostream>

namespace mge {
    std::ostream& operator<<(std::ostream& os, const shader_type& t)
    {
        return os << std::format("{}", t);
    }
} // namespace mge
