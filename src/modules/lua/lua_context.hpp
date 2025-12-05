// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "lua.hpp"
#include "mge/script/script_context.hpp"

namespace mge::lua {

    class lua_engine;

    /**
     * @brief A Lua script context.
     *
     * A Lua script context represents a single execution context for Lua
     * scripts.
     */
    class lua_context : public mge::script_context
    {
    public:
        lua_context(lua_engine* engine);
        ~lua_context();
        void eval(const std::string& code) override;
        int  main(int argc, const char** argv) override;
        void bind() override;

    private:
        lua_engine* m_engine;
        lua_State*  m_lua_state;
    };

} // namespace mge::lua