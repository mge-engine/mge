#include "lua_scope.hpp"
#include "lua_error.hpp"

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

    scope::~scope() {}

} // namespace mge::lua