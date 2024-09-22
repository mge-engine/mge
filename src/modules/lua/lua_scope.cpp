// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "lua_scope.hpp"
#include "lua_error.hpp"

#include "mge/core/checked_cast.hpp"
#include "mge/core/trace.hpp"

namespace mge {
    MGE_USE_TRACE(LUA);
}

namespace mge::lua {
    int scope::new_module(lua_State* L)
    {
        lua_newtable(L);
        return 1;
    }

    scope::scope(lua_context& context, const char* name)
        : m_context(context)
        , m_name(name)
    {
        auto L = m_context.lua_state();
        luaL_requiref(L, m_name, &new_module, 1);
        lua_pop(L, 1);
    }

    scope::scope(lua_context&          context,
                 const lua::scope_ref& parent,
                 const char*           name)
        : m_context(context)
        , m_name(name)
        , m_parent(parent)
    {
        auto L = m_context.lua_state();
        m_parent->load();
        lua_newtable(L);
        lua_setfield(L, -2, name);
        CHECK_CURRENT_STATUS(L);
        lua_pop(L, 1);
    }

    void scope::load()
    {
        auto L = m_context.lua_state();
        if (!m_parent) {
            lua_getglobal(L, m_name);
            CHECK_CURRENT_STATUS(L);
        } else {
            m_parent->load();
            lua_getfield(L, -1, m_name);
            CHECK_CURRENT_STATUS(L);
            lua_remove(L, -2);
        }
    }

    scope::~scope()
    {
        // do not remove any of the created instances
        // creating them again will invoke the gc, and
        // do the needful
    }

    void scope::add(const std::string& name, int64_t value)
    {
        MGE_DEBUG_TRACE(LUA)
            << "Add value '" << name << "' to scope '" << m_name << "'";

        load();

        auto L = m_context.lua_state();

        lua_Integer lua_value = checked_cast<lua_Integer>(value);

        lua_pushinteger(L, lua_value);
        CHECK_CURRENT_STATUS(L);
        lua_setfield(L, -2, name.c_str());
        CHECK_CURRENT_STATUS(L);
        lua_pop(L, 1);
    }

} // namespace mge::lua