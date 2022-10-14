#pragma once
#include "lua.hpp"
#include "lua_context.hpp"
#include "lua_error.hpp"
#include "lua_fwd.hpp"

namespace mge::lua {

    class scope
    {
    public:
        scope(lua_context& context, const char* name);
        scope(lua_context&         context,
              const lua::scope_ref& parent,
              const char*          name);

        virtual ~scope();

        void load();

    private:
        static int new_module(lua_State* L);

        lua_context&  m_context;
        const char*   m_name;
        lua::scope_ref m_parent;
    };

} // namespace mge::lua