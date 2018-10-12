#include "mge/graphics/rectangle.hpp"
#include <iostream>

namespace mge {
    std::ostream&
    operator <<(std::ostream& os, const rectangle& r)
    {
        return os << "{x: " << r.x() << ", y: " << r.y()
                  << ", width: " << r.width()
                  << ", height: " << r.height() << "}";
    }
}
