#pragma once
#include "lua.hpp"
#include "mge/script/script_context.hpp"

namespace mge::lua {
    class lua_context : public script_context
    {
    public:
        lua_context();
        ~lua_context();

    private:
        lua_State* m_lua_state;
    };
} // namespace mge::lua