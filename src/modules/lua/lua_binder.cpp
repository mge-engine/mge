// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "lua_binder.hpp"
#include "lua_context.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/module.hpp"
#include "mge/script/type_details.hpp"
//#include "lua_type.hpp"

#include <stack>

namespace mge {
    MGE_USE_TRACE(LUA);
}

namespace mge::lua {
    lua_binder::lua_binder(lua_context& context)
        : m_context(context)
    {}

    void lua_binder::bind(const mge::script::module& m)
    {
#if 0
        module_binder mb(*this);
        m.apply(mb);
        type_binder tb(*this);
        m.apply(tb);
#endif
    }

} // namespace mge::lua
