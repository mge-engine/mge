#include "lua_type.hpp"
#include "mge/core/details.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/type_details.hpp"

namespace mge {
    MGE_USE_TRACE(LUA);
}

namespace mge::lua {
    type::type(lua_context&                         context,
               const scope_ref&                     parent,
               const mge::script::type_details_ref& t)
        : scope(context, parent, t->name().c_str())
        , m_details(t)
    {
        create_instance_metatable();
        create_type_metatable();
        define_construction();
    }

    void type::create_instance_metatable()
    {
        auto L = m_context.lua_state();
        lua_pushlightuserdata(L, this);
        lua_newtable(L);
        lua_settable(L, LUA_REGISTRYINDEX);
    }

    void type::create_type_metatable()
    {
        auto L = m_context.lua_state();
        lua_pushlightuserdata(L, const_cast<char*>(this->m_name));
        lua_newtable(L);
        lua_settable(L, LUA_REGISTRYINDEX);

        load();
        lua_pushlightuserdata(L, const_cast<char*>(this->m_name));
        lua_gettable(L, LUA_REGISTRYINDEX);
        lua_setmetatable(L, -2);
        lua_pop(L, 1);
    }

    void type::define_construction()
    {
        auto L = m_context.lua_state();
        lua_pushlightuserdata(L, const_cast<char*>(this->m_name));
        lua_gettable(L, LUA_REGISTRYINDEX);
        lua_pushlightuserdata(L, this);
        lua_pushcclosure(L, &construct, 1);
        lua_setfield(L, -2, "__call");
        lua_pop(L, 1);
    }

    int type::construct(lua_State* L)
    {
        // int top = lua_gettop(L);

        void* self_ptr = lua_touserdata(L, lua_upvalueindex(1));
        type* self = reinterpret_cast<type*>(self_ptr);

        MGE_DEBUG_TRACE(LUA) << "Construct " << self->m_details->name();

        return 0;
    }

    int type::destruct(lua_State* L) { return 0; }

} // namespace mge::lua