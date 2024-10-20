// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
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
        scope(lua_context& context, const std::string& name);
        scope(lua_context&          context,
              const lua::scope_ref& parent,
              const char*           name);
        scope(lua_context&          context,
              const lua::scope_ref& parent,
              const std::string&    name);

        virtual ~scope();

        void load();
        void add(const std::string& name, int64_t value);

    private:
        static int new_module(lua_State* L);

    protected:
        lua_context&   m_context;
        std::string    m_name;
        lua::scope_ref m_parent;
    };

} // namespace mge::lua