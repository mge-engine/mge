// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "lua_binder.hpp"
#include "lua_context.hpp"
#include "mge/reflection/function_details.hpp"
#include "mge/reflection/module_details.hpp"
#include "mge/reflection/type_details.hpp"
#include "stack_check_scope.hpp"

namespace mge::lua {

    namespace {
        void load_parent_table(lua_State*                             L,
                               const mge::reflection::module_details& details)
        {
            auto p = details.parent();
            if (p && !p->is_root()) {
                load_parent_table(L, *p);
                lua_getfield(L, -1, p->name().data());
                lua_remove(L, -2); // remove grandparent table
            } else {
                lua_getglobal(L, "_G");
            }
        }
    } // anonymous namespace

    void lua_binder::before(const mge::reflection::module_details& details)
    {
        auto L = m_context->lua_state();

        stack_check_scope scope(L);

        // if module is root, do nothing
        if (!details.is_root()) {
            // create a table for the module
            load_parent_table(L, details);
            lua_newtable(L);
            // add pointer to details as __details__ userdata field
            auto** ud = static_cast<mge::reflection::module_details**>(
                lua_newuserdata(L, sizeof(mge::reflection::module_details*)));
            *ud = const_cast<mge::reflection::module_details*>(&details);
            lua_setfield(L, -2, "__details__");

            // if parent is root, also add to package.loaded
            if (!details.parent() || details.parent()->is_root()) {
                lua_getglobal(L, "package");
                lua_getfield(L, -1, "loaded");
                lua_pushvalue(L, -3); // duplicate the module table
                lua_setfield(L, -2, details.name().data());
                lua_pop(L, 2); // pop package.loaded and package
            }

            lua_setfield(L, -2, details.name().data());
            lua_pop(L, 1); // pop parent table
        }
    }

    void lua_binder::on(const mge::reflection::module_details& details) {}
    void lua_binder::after(const mge::reflection::module_details& details) {}
    void lua_binder::before(const mge::reflection::type_details& details) {}
    void lua_binder::on(const mge::reflection::type_details& details) {}
    void lua_binder::after(const mge::reflection::type_details& details) {}
    void lua_binder::before(const mge::reflection::function_details& details) {}
    void lua_binder::on(const mge::reflection::function_details& details) {}
    void lua_binder::after(const mge::reflection::function_details& details) {}
} // namespace mge::lua