// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/buffer_change_policy.hpp"
#include <iostream>
namespace mge {
    std::ostream& operator <<(std::ostream& os, const buffer_change_policy& u)
    {
        switch(u)
        {
        case buffer_change_policy::DEFAULT:
            return os << "DEFAULT";
        case buffer_change_policy::IMMUTABLE:
            return os << "IMMUTABLE";
        case buffer_change_policy::DYNAMIC:
            return os << "DYNAMIC";
        case buffer_change_policy::STAGING:
            return os << "STAGING";
        default:
            return os << "INVALID(" << (int)u << ")";
        }
    }
}
