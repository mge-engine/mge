#pragma once
#include "lua.hpp"
#include <iosfwd>

namespace mge::lua {

    class lua_datatype
    {
    public:
        lua_datatype()
            : lua_datatype(LUA_TNONE)
        {}

        lua_datatype(int t)
            : m_type(t)
        {}

        lua_datatype(const lua_datatype&) = default;
        lua_datatype& operator=(const lua_datatype&) = default;

        int type() const { return m_type; }

    private:
        int m_type;
    };

    std::ostream& operator<<(std::ostream& os, const lua_datatype& t);

} // namespace mge::lua