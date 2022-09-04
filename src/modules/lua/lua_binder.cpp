// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "lua_binder.hpp"
#include "lua_context.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/module.hpp"
#include "mge/script/module_details.hpp"
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

    class type_creator : public mge::script::visitor
    {
    public:
        type_creator(lua_binder& binder)
            : m_binder(binder)
        {}

        void start(const mge::script::module_details_ref& m) override
        {
            mge::script::module mod(m);

            auto lua_mod = m_binder.context().get_module(mod);
            m_lua_modules.push(lua_mod);
            lua_mod->push_module_table();
        }

        void finish(const mge::script::module_details_ref& m) override
        {
            m_lua_modules.top()->pop_module_table();
            m_lua_modules.pop();
        }

        void start(const mge::script::type_details_ref& t) override
        {
            /*
            if (!t->traits().is_pod()) {
                m_current_type =
                    std::make_shared<python_type>(m_binder.context(), t);
            } else {
                m_current_type.reset();
            }
            */
        }

        void finish(const mge::script::type_details_ref& m) override
        {
            /*
            if (m_current_type) {
                m_binder.add_type(m->type_index(), m_current_type);
                m_current_type.reset();
            }
            */
        }

    private:
        lua_binder&                m_binder;
        std::stack<lua_module_ref> m_lua_modules;
    };

    lua_binder::lua_binder(lua_context& context)
        : m_context(context)
    {}

    void lua_binder::bind(const mge::script::module& m)
    {
        MGE_DEBUG_TRACE(LUA) << "Binding module " << m.name();
        module_binder mb(*this);
        m.apply(mb);
    }

} // namespace mge::lua
