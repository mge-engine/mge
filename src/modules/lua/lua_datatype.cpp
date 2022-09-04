#include "lua_datatype.hpp"
#include <iostream>

namespace mge::lua {

    std::ostream& operator<<(std::ostream& os, const lua_datatype& t)
    {
        switch (t.type()) {
        case -1: // LUA TNONE is wrapped in () and cannot be used
            return os << "NONE";
            break;
        case LUA_TNIL:
            return os << "NIL";
            break;
        default:
            return os << "INVALID(" << t.type() << ")";
        }
    }
} // namespace mge::lua
