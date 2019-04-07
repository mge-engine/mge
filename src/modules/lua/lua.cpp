// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/script/script_engine.hpp"
#include "lua_context.hpp"

namespace lua {
    class lua_engine : public mge::script_engine
    {
    public:
        lua_engine() = default;
        virtual  ~lua_engine() = default;
        mge::script_context_ref create_context()
        {
            mge::script_context_ref result = std::make_shared<lua_context>();
            return result;
        }
    };

    MGE_REGISTER_IMPLEMENTATION(lua_engine, mge::script_engine, lua);
}
