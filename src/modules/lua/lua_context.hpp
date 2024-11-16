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
        void eval(const std::string& code) override;
        int  main(int argc, const char** argv) override;
        void bind() override;

        lua_State* lua_state() const noexcept { return m_lua_state; }
        const lua::module_ref&
        module_from_module_data(const mge::script::module_data_ref& m) const;

    private:
        void bind_module(const mge::script::module_data_ref& data);
        void bind_helper_module();
        bool is_builtin(const mge::script::type_data_ref& t) const;

        lua_State*                                          m_lua_state;
        std::vector<lua::module_ref>                        m_modules;
        std::map<mge::script::type_data_ref, lua::type_ref> m_types;
    };
} // namespace mge::lua