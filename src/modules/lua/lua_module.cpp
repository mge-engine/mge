// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "lua_module.hpp"
#include "lua_context.hpp"

namespace mge::lua {
    lua_module::lua_module(lua_context& context, const mge::script::module& m)
        : m_context(context)
        , m_module(m)
    {}

    lua_module::~lua_module()
    {
        if (!m_module.is_root()) {
            remove_module_from_parent();
        }
    }

    void lua_module::remove_module_from_parent() {}

} // namespace mge::lua