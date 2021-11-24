// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "lua_error.hpp"

namespace mge::lua {
    MGE_DEFINE_EXCEPTION_CLASS(error);

    void error::check_status(int status, lua_State* state)
    {
        if (status == LUA_OK) {
            return;
        }

        std::string errmsg(lua_tostring(state, -1));
        MGE_THROW(mge::lua::error) << "Lua error: " << errmsg;
    }
} // namespace mge::lua