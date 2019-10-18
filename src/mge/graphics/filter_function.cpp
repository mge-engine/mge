// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/filter_function.hpp"
#include <iostream>
namespace mge {
    
    std::ostream& operator <<(std::ostream& os, const filter_function& f)
    {
        switch(f) {
        case filter_function::LINEAR:
            return os << "LINEAR";
        case filter_function::NEAREST:
            return os << "NEAREST";
        default:
            return os << "INVALID(" << (int)f << ")";
        }
    }
}
