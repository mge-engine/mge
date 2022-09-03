// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "lua.hpp"
#include "lua_module.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/script_context.hpp"

#include <unordered_map>

namespace mge::lua {

    class lua_context : public script_context
    {
    public:
        lua_context();
        ~lua_context();
        void eval(const std::string& code) override;
        int  main(int argc, const char** argv) override;
        void bind(const mge::script::module& m) override;

        lua_State*     lua_state() const { return m_lua_state; }
        lua_module_ref get_or_add_module(const mge::script::module& m);

    private:
        std::unordered_map<mge::script::module, lua_module_ref> m_lua_modules;

        lua_State* m_lua_state;
    };
} // namespace mge::lua