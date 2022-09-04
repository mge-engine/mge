// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "lua_type.hpp"
#include "mge/script/module.hpp"
#include "mge/script/visitor.hpp"

namespace mge::lua {

    class lua_context;

    class lua_binder
    {
    public:
        lua_binder(lua_context& context);
        ~lua_binder() = default;

        void bind(const mge::script::module& m);

        const lua_context& context() const noexcept { return m_context; }
        lua_context&       context() noexcept { return m_context; }

        void add_type(std::type_index i, const lua::type_ref& t)
        {
            m_created_types.emplace(i, t);
        }

    private:
        lua_context&                             m_context;
        std::map<std::type_index, lua::type_ref> m_created_types;
    };
} // namespace mge::lua