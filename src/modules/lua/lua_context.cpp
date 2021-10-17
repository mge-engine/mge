#include "lua_context.hpp"

namespace mge::lua {
    lua_context::lua_context()
        : m_lua_state(nullptr)
    {
        m_lua_state = luaL_newstate();
        luaL_openlibs(m_lua_state);
    }

    lua_context::~lua_context()
    {
        if (m_lua_state) {
            lua_close(m_lua_state);
        }
    }

} // namespace mge::lua