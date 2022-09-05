// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "lua_binder.hpp"
#include "lua_context.hpp"
#include "lua_module.hpp"
#include "lua_type.hpp"

#include "mge/core/trace.hpp"
#include "mge/script/module.hpp"
#include "mge/script/module_details.hpp"
#include "mge/script/type_details.hpp"

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
            MGE_DEBUG_TRACE(LUA)
                << "Start processing for type '" << t->name() << "'";
            if (!t->traits().is_pod() && m_lua_modules.top()->has_lua_table()) {
                auto lt = std::make_shared<lua::type>(m_binder.context(), t);
                m_lua_types.push(lt);
            } else {
                m_lua_types.push(lua::type_ref());
            }
        }

        void finish(const mge::script::type_details_ref& t) override
        {
            auto lt = m_lua_types.top();
            m_lua_types.pop();
            if (lt) {
                MGE_DEBUG_TRACE(LUA) << "Add type '" << t->name() << "' ";
                m_binder.add_type(t->type_index(), lt);
            }
        }

        void enum_value(const std::string& name, int64_t value)
        {
            if (m_lua_types.top()) {
                m_lua_types.top()->add_enum_value(name, value);
            }
        }

    private:
        lua_binder&                m_binder;
        std::stack<lua_module_ref> m_lua_modules;
        std::stack<lua::type_ref>  m_lua_types;
    };

    lua_binder::lua_binder(lua_context& context)
        : m_context(context)
    {}

    void lua_binder::bind(const mge::script::module& m)
    {
        MGE_DEBUG_TRACE(LUA) << "Binding module " << m.name();
        module_binder mb(*this);
        m.apply(mb);
        MGE_DEBUG_TRACE(LUA) << "Creating types of " << m.name();
        type_creator tc(*this);
        m.apply(tc);
    }

} // namespace mge::lua
