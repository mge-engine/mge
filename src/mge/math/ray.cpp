#include "mge/math/ray.hpp"
#include <iostream>

namespace mge {
    std::ostream&
    operator <<(std::ostream& os, const ray& r)
    {
        return os << "ray[origin=" << r.origin() << ", direction=" << r.direction() << "]";
    }
}
