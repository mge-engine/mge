// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "lua_context.hpp"
#include "lua_error.hpp"
#include "mge/core/trace.hpp"

namespace mge {
    MGE_USE_TRACE(LUA);
}

namespace mge::lua {

    lua_context::lua_context(lua_engine* engine)
        : m_engine(engine)
        , m_lua_state(nullptr)
    {
        MGE_DEBUG_TRACE(LUA, "Creating new Lua state");
        m_lua_state = luaL_newstate();
        CHECK_STATUS(m_lua_state ? LUA_OK : LUA_ERRMEM, m_lua_state);
        MGE_DEBUG_TRACE(LUA, "Lua state created successfully");
        luaL_openlibs(m_lua_state);
        MGE_DEBUG_TRACE(LUA, "Lua libs opened successfully");
    }

    lua_context::~lua_context()
    {
        if (m_lua_state) {
            lua_close(m_lua_state);
            m_lua_state = nullptr;
        }
    }

    void lua_context::eval(const std::string& code)
    {
        int rc = luaL_loadbuffer(m_lua_state, code.data(), code.size(), "");
        CHECK_STATUS(rc, m_lua_state);
        rc = lua_pcall(m_lua_state, 0, 0, 0);
        CHECK_STATUS(rc, m_lua_state);
    }

    void lua_context::bind() {}

    int lua_context::main(int argc, const char** argv)
    {
#if 0
        int        status, result;
        lua_State* L = m_lua_state;
        if (L == NULL) {
            l_message(argv[0], "cannot create state: not enough memory");
            return EXIT_FAILURE;
        }
        lua_pushcfunction(L, &pmain);   /* to call 'pmain' in protected mode */
        lua_pushinteger(L, argc);       /* 1st argument */
        lua_pushlightuserdata(L, argv); /* 2nd argument */
        status = lua_pcall(L, 2, 1, 0); /* do the call */
        result = lua_toboolean(L, -1);  /* get result */
        report(L, status);
        lua_close(L);
        return (result && status == LUA_OK) ? EXIT_SUCCESS : EXIT_FAILURE;
#endif
        return 0;
    }

} // namespace mge::lua