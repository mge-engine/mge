// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/memory.hpp"
#include "mge/script/module.hpp"

#include "lua.hpp"
#include "lua_fwd.hpp"

#include <vector>

namespace mge::lua {

    class lua_module
    {
    public:
        lua_module(lua_context& context, const mge::script::module& m);
        ~lua_module();
        bool has_lua_table() const { return m_has_lua_table; }

        void push_module_table();
        void pop_module_table();

        void add_type(const lua::type_ref& t);

    private:
        void add_global_module();
        void add_submodule();
        void remove_module_from_parent();
        void load_parent();

        lua_context&               m_context;
        mge::script::module        m_module;
        std::vector<lua::type_ref> m_types;
        bool                       m_has_lua_table;
        bool                       m_is_global;
    };

} // namespace mge::lua