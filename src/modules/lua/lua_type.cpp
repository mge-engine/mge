#include "lua_type.hpp"
#include "mge/script/module_data.hpp"
#include "mge/script/type_data.hpp"

namespace mge {
    MGE_USE_TRACE(LUA);
}

namespace mge::lua {

    type::type(lua_context& context, const mge::script::type_data_ref& t)
        : m_context(context)
        , m_name_in_module(t->exposed_name())
        , m_type(t)
    {
        if (m_type->exposed_directly()) {
            m_module_name = m_type->module().lock()->full_name();
        } else {
            m_module_name = "__mge__";
        }
        MGE_DEBUG_TRACE(LUA) << "Creating type " << t->name();
        set_type_in_module();
    }

    void type::create_type_metatable()
    {
        auto L = m_context.lua_state();
        lua_pushlightuserdata(L, this);
        lua_newtable(L);
        lua_settable(L, LUA_REGISTRYINDEX);
    }

    void type::set_type_in_module()
    {
        auto m = m_type->module().lock();
        auto lm = m_context.module_from_module_data(m);
    }
} // namespace mge::lua