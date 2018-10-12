#include "mge/graphics/extent.hpp"
#include <iostream>

namespace mge {

std::ostream& operator <<(std::ostream& os, const extent& e)
{
    return os << "{width: " << e.width() << ", height: " << e.height() << "}";
}

}
