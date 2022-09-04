// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/memory.hpp"
#include "mge/script/module.hpp"

#include "lua.hpp"

namespace mge::lua {

    class lua_context;

    MGE_DECLARE_REF(lua_module);

    class lua_module
    {
    public:
        lua_module(lua_context& context, const mge::script::module& m);
        ~lua_module();

    private:
        void add_global_module();
        void add_submodule();
        void remove_module_from_parent();
        void load_parent();

        lua_context&        m_context;
        mge::script::module m_module;
    };

} // namespace mge::lua