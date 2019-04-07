// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/buffer_type.hpp"
#include <iostream>

namespace mge {
    std::ostream& operator <<(std::ostream& os, const buffer_type& t)
    {
        switch (t)
        {
        case buffer_type::VERTEX_BUFFER:
            return os << "VERTEX_BUFFER";
        case buffer_type::INDEX_BUFFER:
            return os << "INDEX_BUFFER";
        case buffer_type::CONSTANT_BUFFER:
            return os << "CONSTANT_BUFFER";
        default:
            return os << "INVALID(" << (int)t << ")";
        }
    }
}
