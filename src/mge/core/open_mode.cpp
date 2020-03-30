// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/open_mode.hpp"
#include <iostream>
namespace mge {
    std::ostream &operator<<(std::ostream &os, const open_mode &m)
    {
        switch (m) {
        case open_mode::READ:
            return os << "READ";
        case open_mode::WRITE:
            return os << "WRITE";
        case open_mode::READ_WRITE:
            return os << "READ_WRITE";
        default:
            return os << "INVALID(" << (int)m << ")";
        }
    }
} // namespace mge