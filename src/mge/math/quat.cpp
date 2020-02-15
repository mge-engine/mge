#include "mge/math/quat.hpp"
#include <iostream>

namespace mge {

    std::ostream& operator <<(std::ostream& os, const fquat& w)
    {
        return os << "[" << w.x << ", " << w.y << ", " << w.z << ", " << w.w << "]";
    }

}
