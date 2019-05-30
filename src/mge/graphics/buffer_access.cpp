#include "mge/graphics/buffer_access.hpp"
#include <iostream>
namespace mge {
    std::ostream&
    operator <<(std::ostream& os, const buffer_access a)
    {
        switch(a) {
        case buffer_access::NONE:
            return os << "NONE";
        case buffer_access::READ:
            return os << "READ";
        case buffer_access::WRITE:
            return os << "WRITE";
        case buffer_access::READ_WRITE:
            return os << "READ_WRITE";
        default:
            return os << "INVALID(" << (uint8_t)a << ")";
        }
    }
}
