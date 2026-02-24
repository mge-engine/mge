// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "lua_binder.hpp"
#include "lua_context.hpp"
#include "mge/core/trace.hpp"
#include "mge/reflection/function_details.hpp"
#include "mge/reflection/module_details.hpp"
#include "mge/reflection/type_details.hpp"
#include "stack_check_scope.hpp"

namespace mge {
    MGE_USE_TRACE(LUA);
}

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
            // add pointer to details as __details__ light user data field
            lua_pushlightuserdata(
                L,
                const_cast<mge::reflection::module_details*>(&details));
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
            MGE_DEBUG_TRACE(LUA,
                            "Registered module to Lua: {}",
                            details.full_name().c_str());
        }
    }

    void lua_binder::on(const mge::reflection::module_details& details)
    {
        auto L = m_context->lua_state();

        if (!details.is_root()) {
            // load the module table onto the stack
            load_parent_table(L, details);
            lua_getfield(L, -1, details.name().data());
            lua_remove(L, -2); // remove parent table
            MGE_DEBUG_TRACE(LUA,
                            "Loaded module table for: {}",
                            details.full_name().c_str());
        }
    }

    void lua_binder::after(const mge::reflection::module_details& details)
    {
        auto L = m_context->lua_state();

        if (!details.is_root()) {
            // pop the module table
            lua_pop(L, 1);
        }
    }

    void lua_binder::before(const mge::reflection::type_details& details) {}

    void lua_binder::on(const mge::reflection::type_details& details)
    {
        MGE_DEBUG_TRACE(LUA,
                        "Binding type: {}",
                        std::string(details.name).c_str());
        if (details.is_enum) {
            bind_enum(details.name, details.enum_specific());
        } else if (details.is_class) {
            bind_class(details);
        }
    }

    void lua_binder::bind_enum(
        std::string_view                                            name,
        const mge::reflection::type_details::enum_specific_details& details)
    {
        auto L = m_context->lua_state();
        // parent module table is on top of the stack
        lua_newtable(L);
        for (const auto& [val_name, val] : details.values) {
            lua_pushstring(L, val_name.data());
            if (std::holds_alternative<int64_t>(val)) {
                lua_pushinteger(
                    L,
                    static_cast<lua_Integer>(std::get<int64_t>(val)));
            } else {
                lua_pushinteger(
                    L,
                    static_cast<lua_Integer>(std::get<uint64_t>(val)));
            }
            lua_settable(L, -3);
        }
        lua_setfield(L, -2, name.data());
        MGE_DEBUG_TRACE(LUA, "Bound enum: {}", std::string(name).c_str());
    }

    void lua_binder::bind_class(const mge::reflection::type_details& details)
    {
        auto             L = m_context->lua_state();
        std::string_view n = details.name;
        // strip namespace prefix (e.g. "mge::point" -> "point")
        auto pos = n.rfind("::");
        if (pos != std::string_view::npos) {
            n = n.substr(pos + 2);
        }
        // parent module table is on top of the stack
        lua_newtable(L);
        std::string short_name(n);
        lua_setfield(L, -2, short_name.c_str());
        MGE_DEBUG_TRACE(LUA,
                        "Bound class: {}",
                        std::string(details.name).c_str());
    }

    void lua_binder::after(const mge::reflection::type_details& details) {}
    void lua_binder::before(const mge::reflection::function_details& details) {}
    void lua_binder::on(const mge::reflection::function_details& details) {}
    void lua_binder::after(const mge::reflection::function_details& details) {}
} // namespace mge::lua