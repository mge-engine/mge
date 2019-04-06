// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/extent.hpp"
#include <iostream>

namespace mge {

std::ostream& operator <<(std::ostream& os, const extent& e)
{
    return os << "extent[width=" << e.width() << ", height=" << e.height() << "]";
}

}
