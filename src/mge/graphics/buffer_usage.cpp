// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/buffer_usage.hpp"
#include <iostream>
namespace mge {
    std::ostream& operator <<(std::ostream& os, const buffer_usage& u)
    {
        switch(u)
        {
        case buffer_usage::DEFAULT:
            return os << "DEFAULT";
        case buffer_usage::IMMUTABLE:
            return os << "IMMUTABLE";
        case buffer_usage::DYNAMIC:
            return os << "DYNAMIC";
        case buffer_usage::STREAMING:
            return os << "STREAMING";
        default:
            return os << "INVALID(" << (int)u << ")";
        }
    }
}
