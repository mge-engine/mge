// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "lua_module.hpp"
#include "lua_context.hpp"

namespace mge {
    MGE_USE_TRACE(LUA);
}

namespace mge::lua {

    lua_module::lua_module(lua_context& context, const mge::script::module& m)
        : m_context(context)
        , m_module(m)
    {
        if (m.is_root()) {
            MGE_DEBUG_TRACE(LUA) << "Not adding root module to Lua";
        } else {
            if (m.parent().is_root()) {
                if (m.name() == "std") {
                    MGE_DEBUG_TRACE(LUA) << "Not adding 'std' module to Lua";
                } else {
                    add_global_module();
                }
            } else {
                add_submodule();
            }
        }
    }

    static int new_module(lua_State* L)
    {
        lua_newtable(L);
        return 1;
    }

    void lua_module::add_global_module()
    {
        MGE_DEBUG_TRACE(LUA) << "Add global module " << m_module.name();

        auto L = m_context.lua_state();
        luaL_requiref(L, m_module.name().c_str(), &new_module, 1);
        lua_pop(L, 1);
    }

    void lua_module::add_submodule() {}

    lua_module::~lua_module()
    {
        if (!m_module.is_root()) {
            remove_module_from_parent();
        }
    }

    void lua_module::remove_module_from_parent() {}

} // namespace mge::lua