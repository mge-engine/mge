// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "lua.hpp"
#include "mge/reflection/module_details.hpp"
namespace mge::lua {

    class lua_module
    {
    public:
        lua_module(const mge::reflection::module_details& details);
        ~lua_module();

        lua_module(const lua_module&) = delete;
        lua_module& operator=(const lua_module&) = delete;
        lua_module(lua_module&&) = default;
        lua_module& operator=(lua_module&&) = default;

    private:
        const mge::reflection::module_details* m_details;
    };

} // namespace mge::lua