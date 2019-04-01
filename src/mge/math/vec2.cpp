#include "mge/math/vec2.hpp"
#include <iostream>

namespace mge {
    std::ostream& operator <<(std::ostream& os, const fvec2& v)
    {
        return os << "[" << v.x << ", " << v.y << "]";
    }
}