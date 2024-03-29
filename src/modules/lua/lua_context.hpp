// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "lua.hpp"
#include "lua_fwd.hpp"

#include "mge/core/trace.hpp"
#include "mge/script/script_context.hpp"

#include <unordered_map>

namespace mge::lua {

    class lua_context : public script_context
    {
    public:
        lua_context();
        ~lua_context();
        void            eval(const std::string& code) override;
        int             main(int argc, const char** argv) override;
        void            bind(const mge::script::module& m) override;
        script_type_ref type_of(const std::string& expression) override;

        lua_State* lua_state() const { return m_lua_state; }
        void       details(std::ostream& os) const;

        void add_module(const lua::module_ref& m);
        void add_type(const lua::type_ref& t);

    private:
        std::vector<lua::module_ref> m_all_modules;
        std::vector<lua::type_ref>   m_all_types;
        lua_State*                   m_lua_state;
    };
} // namespace mge::lua