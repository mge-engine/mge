// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "lua.hpp"
#include "lua_context.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/script_engine.hpp"

namespace mge {
    MGE_DEFINE_TRACE(LUA);
}

namespace mge::lua {
    class lua_engine : public script_engine
    {
    public:
        lua_engine()
        {
            MGE_INFO_TRACE(LUA, "Lua script engine created");
        }

        ~lua_engine() = default;

        script_context_ref create_context() override
        {
            script_context_ref result = std::make_shared<lua_context>(this);
            return result;
        }
    };
} // namespace mge::lua