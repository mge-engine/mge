// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/usage.hpp"
#include <iostream>
namespace mge {
    std::ostream& operator <<(std::ostream& os, const usage& u)
    {
        switch(u)
        {
        case usage::DEFAULT:
            return os << "DEFAULT";
        case usage::IMMUTABLE:
            return os << "IMMUTABLE";
        case usage::DYNAMIC:
            return os << "DYNAMIC";
        case usage::STAGING:
            return os << "STAGING";
        default:
            return os << "INVALID(" << (int)u << ")";
        }
    }
}
