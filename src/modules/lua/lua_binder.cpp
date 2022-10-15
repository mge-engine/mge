// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "lua_binder.hpp"
#include "lua_context.hpp"
#include "lua_module.hpp"
#include "lua_type.hpp"

#include "mge/core/details.hpp"
#include "mge/core/overloaded.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/module.hpp"
#include "mge/script/module_details.hpp"
#include "mge/script/type_details.hpp"

#include <stack>

namespace mge {
    MGE_USE_TRACE(LUA);
}

namespace mge::lua {

    class scope_creator : public mge::script::visitor
    {
    public:
        scope_creator(lua_binder& binder)
            : m_binder(binder)
        {}

        using mge::script::visitor::finish;
        using mge::script::visitor::start;

        virtual void start(const mge::script::module_details_ref& m) override
        {
            if (!m->is_root()) {
                MGE_DEBUG_TRACE(LUA) << "Create module '" << m->name() << "'";
                lua::module_ref current;
                if (m->parent()->is_root()) {
                    current =
                        std::make_shared<lua::module>(m_binder.context(), m);
                } else {
                    current = std::make_shared<lua::module>(m_binder.context(),
                                                            m_definitions.top(),
                                                            m);
                }
                m_binder.context().add_module(current);
                m_definitions.push(current);
                m_definition_pushed.push(true);
            } else {
                m_definition_pushed.push(false);
            }
        };

        virtual void start(const mge::script::type_details_ref& t) override
        {
            if (!t->traits().is_pod() && !m_definitions.empty()) {
                MGE_DEBUG_TRACE(LUA) << "Create type '" << t->name() << "'";
                auto current = std::make_shared<lua::type>(m_binder.context(),
                                                           m_definitions.top(),
                                                           t);
                m_binder.context().add_type(current);
                m_definitions.push(current);
                m_definition_pushed.push(true);
                m_type_definitions.push(current);
            } else {
                m_definition_pushed.push(false);
                m_type_definitions.push(lua::type_ref());
            }
        }

        virtual void finish(const mge::script::type_details_ref&) override
        {
            if (m_definition_pushed.top()) {
                m_definitions.pop();
            }
            m_definition_pushed.pop();
            m_type_definitions.pop();
        }

        virtual void finish(const mge::script::module_details_ref&) override
        {
            if (m_definition_pushed.top()) {
                m_definitions.pop();
            }
            m_definition_pushed.pop();
        }

        void enum_value(const std::string& name, int64_t value)
        {
            if (m_definition_pushed.top()) {
                m_definitions.top()->add(name, value);
            }
        }

        void
        constructor(const mge::script::signature&       s,
                    const mge::script::invoke_function& new_at,
                    const mge::script::invoke_function& make_shared) override
        {
            const auto& t = m_type_definitions.top();
            if (t) {
                t->add_constructor(s, new_at, make_shared);
            }
        }

        void destructor(
            const mge::script::invoke_function& delete_ptr,
            const mge::script::invoke_function& delete_shared_ptr) override
        {
            const auto& t = m_type_definitions.top();
            if (t) {
                t->set_destructor(delete_ptr, delete_shared_ptr);
            }
        }

    private:
        lua_binder&                m_binder;
        std::stack<lua::scope_ref> m_definitions;
        std::stack<lua::type_ref>  m_type_definitions;
        std::stack<bool>           m_definition_pushed;
    };

#if 0

    class type_fields_creator : public mge::script::visitor
    {
    public:
        type_fields_creator(lua_binder& binder)
            : m_binder(binder)
        {}

        void start(const mge::script::module_details_ref& m) override
        {
            MGE_DEBUG_TRACE(LUA)
                << "Setting type fields of '" << m->name() << "'";
            mge::script::module mod(m);

            auto lua_mod = m_binder.context().get_module(mod);
            m_definitions.push(lua_mod);
            lua_mod->push_module_table();
        }

        void finish(const mge::script::module_details_ref& m) override
        {
            pop_from_lua_stack();
            m_definitions.pop();
        }

        void start(const mge::script::type_details_ref& t) override
        {
            if (!t->traits().is_pod() && parent_in_lua()) {
                MGE_DEBUG_TRACE(LUA)
                    << "Setting fields of type '" << t->name() << "'";
                auto lt = m_binder.get_type(t->type_index());
                m_definitions.push(lt);
                if (lt->materialized()) {

                    lt->push_type_table();
                }
            } else {
                m_definitions.push(lua::type_ref());
            }
        }


        void field(const std::string&                   name,
                   const mge::script::type_details_ref& type,
                   const mge::script::invoke_function&  setter,
                   const mge::script::invoke_function&  getter) override
        {
            const auto& t = std::get<lua::type_ref>(m_definitions.top());
            if (t) {
                t->add_field(name, type, setter, getter);
            }
        }

        void
        constructor(const mge::script::signature&       s,
                    const mge::script::invoke_function& new_at,
                    const mge::script::invoke_function& make_shared) override
        {
            const auto& t = std::get<lua::type_ref>(m_definitions.top());
            if (t) {
                t->add_constructor(s, new_at, make_shared);
            }
        }

        void destructor(
            const mge::script::invoke_function& delete_ptr,
            const mge::script::invoke_function& delete_shared_ptr) override
        {
            const auto& t = std::get<lua::type_ref>(m_definitions.top());
            if (t) {
                t->set_destructor(delete_ptr, delete_shared_ptr);
            }
        }

        void finish(const mge::script::type_details_ref& t) override
        {
            MGE_DEBUG_TRACE(LUA) << "Finish type " << t->name();
            pop_from_lua_stack();
            m_definitions.pop();
        }

        void method(const std::string&                  name,
                    bool                                is_static,
                    const std::type_index&              return_type,
                    const mge::script::signature&       sig,
                    const mge::script::invoke_function& invoke) override
        {
            const auto& t = std::get<lua::type_ref>(m_definitions.top());
            if (t) {
                if (is_static) {
                    t->add_static_method(name, return_type, sig, invoke);
                } else {
                    t->add_method(name, return_type, sig, invoke);
                }
            }
        }

    private:
        void pop_from_lua_stack()
        {
            std::visit(overloaded{[](const std::monostate&) {},
                                  [](const lua_module_ref& r) {
                                      if (r && r->has_lua_table()) {
                                          r->pop_module_table();
                                      }
                                  },
                                  [](const lua::type_ref& t) {
                                      if (t && t->materialized()) {
                                          t->pop_type_table();
                                      }
                                  }},
                       m_definitions.top());
        }

        bool parent_in_lua() const
        {
            return std::visit(
                overloaded{[](const std::monostate&) -> bool { return false; },
                           [](const lua_module_ref& r) -> bool {
                               if (r) {
                                   return r->has_lua_table();
                               } else {
                                   return false;
                               }
                           },
                           [](const lua::type_ref& t) -> bool {
                               if (t) {
                                   return t->materialized();
                               } else {
                                   return false;
                               }
                           }},
                m_definitions.top());
        }

        lua_binder& m_binder;

        using element =
            std::variant<std::monostate, lua_module_ref, lua::type_ref>;
        std::stack<element> m_definitions;
    };
#endif

    lua_binder::lua_binder(lua_context& context)
        : m_context(context)
    {}

    void lua_binder::bind(const mge::script::module& m)
    {
        MGE_DEBUG_TRACE(LUA) << "Binding module '" << m.name() << "'";
        scope_creator mb(*this);
        m.apply(mb);
#if 0
        type_creator tc(*this);
        m.apply(tc);
        type_fields_creator tfc(*this);
        m.apply(tfc);
#endif
    }

} // namespace mge::lua
