// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.

#include "lua_context.hpp"
#include "lua_error.hpp"

#include <string>
#include <string_view>
#include <vector>

using namespace std::string_view_literals;

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

    int lua_context::main(int, const char**) { return 0; }

    void lua_context::eval(const std::string& code)
    {
        int rc = luaL_loadbuffer(m_lua_state, code.data(), code.size(), "");
        CHECK_STATUS(rc, m_lua_state);
        rc = lua_pcall(m_lua_state, 0, 0, 0);
        CHECK_STATUS(rc, m_lua_state);
    }

    void lua_context::bind(const mge::script::module& m) {}

} // namespace mge::lua