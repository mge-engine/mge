// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "lua_binder.hpp"
#include "mge/reflection/function_details.hpp"
#include "mge/reflection/module_details.hpp"
#include "mge/reflection/type_details.hpp"

namespace mge::lua {

    void lua_binder::before(const mge::reflection::module_details& details) {}

    void lua_binder::on(const mge::reflection::module_details& details) {}
    void lua_binder::after(const mge::reflection::module_details& details) {}
    void lua_binder::before(const mge::reflection::type_details& details) {}
    void lua_binder::on(const mge::reflection::type_details& details) {}
    void lua_binder::after(const mge::reflection::type_details& details) {}
    void lua_binder::before(const mge::reflection::function_details& details) {}
    void lua_binder::on(const mge::reflection::function_details& details) {}
    void lua_binder::after(const mge::reflection::function_details& details) {}
} // namespace mge::lua