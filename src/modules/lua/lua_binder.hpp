// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "lua.hpp"
#include "mge/reflection/visitor.hpp"
#include "mge/script/script_context.hpp"

namespace mge::lua {

    class lua_context;

    class lua_binder : public mge::reflection::visitor
    {
    public:
        lua_binder(lua_context* context)
            : m_context(context)
        {}

        ~lua_binder() = default;

        void before(const mge::reflection::module_details& details) override;
        void on(const mge::reflection::module_details& details) override;
        void after(const mge::reflection::module_details& details) override;

        void before(const mge::reflection::type_details& details) override;
        void on(const mge::reflection::type_details& details) override;
        void after(const mge::reflection::type_details& details) override;

        void before(const mge::reflection::function_details& details) override;
        void on(const mge::reflection::function_details& details) override;
        void after(const mge::reflection::function_details& details) override;

    private:
        lua_context* m_context;
    };

} // namespace mge::lua