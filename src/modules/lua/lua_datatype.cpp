// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
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
        case LUA_TBOOLEAN:
            return os << "BOOLEAN";
        case LUA_TLIGHTUSERDATA:
            return os << "LIGHTUSERDATA";
        case LUA_TNUMBER:
            return os << "NUMBER";
        case LUA_TSTRING:
            return os << "STRING";
        case LUA_TTABLE:
            return os << "TABLE";
        case LUA_TFUNCTION:
            return os << "FUNCTION";
        case LUA_TUSERDATA:
            return os << "USERDATA";
        case LUA_TTHREAD:
            return os << "THREAD";
        default:
            return os << "INVALID(" << t.type() << ")";
        }
    }

} // namespace mge::lua
