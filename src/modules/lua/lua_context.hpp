// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "lua.hpp"
#include "mge/script/script_context.hpp"

namespace mge::lua {
    class lua_context : public script_context
    {
    public:
        lua_context();
        ~lua_context();
        void eval(std::string_view code);

    private:
        lua_State* m_lua_state;
    };
} // namespace mge::lua