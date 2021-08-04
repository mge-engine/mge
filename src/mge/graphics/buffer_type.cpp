// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/buffer_type.hpp"
#include <iostream>

namespace mge {

    std::ostream& operator<<(std::ostream& os, const buffer_type& t)
    {
        switch (t) {
        case buffer_type::VERTEX:
            return os << "VERTEX";
        case buffer_type::INDEX:
            return os << "INDEX";
        case buffer_type::CONSTANT:
            return os << "CONSTANT";
        default:
            return os << "INVALID(" << static_cast<uint32_t>(t) << ")";
        }
    }

} // namespace mge