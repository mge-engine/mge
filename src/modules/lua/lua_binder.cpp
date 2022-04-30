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

    class module_binder : public mge::script::visitor
    {
    public:
        using mge::script::visitor::start;

        module_binder(lua_binder& binder)
            : m_binder(binder)
        {}

        virtual void start(const mge::script::module_details_ref& m) override
        {
            mge::script::module mod(m);
            m_binder.context().get_or_add_module(mod);
        }

        lua_binder& m_binder;
    };

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
