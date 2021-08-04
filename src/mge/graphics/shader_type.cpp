// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/shader_type.hpp"
#include <iostream>
namespace mge {
    std::ostream& operator<<(std::ostream& os, const shader_type& t)
    {
        switch (t) {
        case shader_type::VERTEX:
            return os << "VERTEX";
        case shader_type::FRAGMENT:
            return os << "FRAGMENT";
        case shader_type::COMPUTE:
            return os << "COMPUTE";
        case shader_type::TESSELATION_CONTROL:
            return os << "TESSELATION_CONTROL";
        case shader_type::TESSELATION_EVALUATION:
            return os << "TESSELATION_EVALUATION";
        case shader_type::GEOMETRY:
            return os << "GEOMETRY";
        default:
            return os << "INVALID(" << (int)t << ")";
        }
    }

} // namespace mge
