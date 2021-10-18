// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
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
        lua_engine() { MGE_INFO_TRACE(LUA) << "Create Lua script engine"; }
        virtual ~lua_engine() = default;

        script_context_ref create_context()
        {
            script_context_ref result = std::make_shared<lua_context>();
            return result;
        }
    };

    MGE_REGISTER_IMPLEMENTATION(lua_engine, mge::script_engine, lua);
} // namespace mge::lua