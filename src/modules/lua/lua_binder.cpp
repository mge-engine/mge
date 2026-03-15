// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "lua_binder.hpp"
#include "lua_call_context.hpp"
#include "lua_context.hpp"
#include "mge/core/trace.hpp"
#include "mge/reflection/function_details.hpp"
#include "mge/reflection/module_details.hpp"
#include "mge/reflection/type_details.hpp"
#include "mge/reflection/type_identifier.hpp"
#include "stack_check_scope.hpp"

#include <typeindex>

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
        std::string_view n =
            details.alias.empty() ? details.name : details.alias;
        // strip namespace prefix (e.g. "mge::point" -> "point")
        auto pos = n.rfind("::");
        if (pos != std::string_view::npos) {
            n = n.substr(pos + 2);
        }
        std::string short_name(n);

        // create instance metatable first (stored in registry)
        create_instance_metatable(details);

        // parent module table is on top of the stack
        // create class table
        lua_newtable(L);

        // store type_details pointer
        lua_pushlightuserdata(
            L,
            const_cast<mge::reflection::type_details*>(&details));
        lua_setfield(L, -2, "__details__");

        // set metatable on class table with __call for construction
        lua_newtable(L); // metatable for class table
        lua_pushcfunction(L, &lua_binder::class_call);
        lua_setfield(L, -2, "__call");
        lua_setmetatable(L, -2);

        // bind static methods as functions on the class table
        const auto& class_details =
            std::get<mge::reflection::type_details::class_specific_details>(
                details.specific_details);
        for (size_t i = 0; i < class_details.static_methods.size(); ++i) {
            const auto& [name, sig, invoke_fn] =
                class_details.static_methods[i];
            // upvalue 1: type_details pointer
            lua_pushlightuserdata(
                L,
                const_cast<mge::reflection::type_details*>(&details));
            // upvalue 2: method index
            lua_pushinteger(L, static_cast<lua_Integer>(i));
            lua_pushcclosure(L, &lua_binder::static_method_call, 2);
            lua_setfield(L, -2, std::string(name).c_str());
        }

        lua_setfield(L, -2, short_name.c_str());
        MGE_DEBUG_TRACE(LUA,
                        "Bound class: {}",
                        std::string(details.name).c_str());
    }

    void lua_binder::create_instance_metatable(
        const mge::reflection::type_details& details)
    {
        auto L = m_context->lua_state();

        // use type_details pointer as registry key
        lua_pushlightuserdata(
            L,
            const_cast<mge::reflection::type_details*>(&details));
        lua_newtable(L); // the metatable

        // store type_details in metatable
        lua_pushlightuserdata(
            L,
            const_cast<mge::reflection::type_details*>(&details));
        lua_setfield(L, -2, "__type__");

        lua_pushcfunction(L, &lua_binder::instance_index);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, &lua_binder::instance_newindex);
        lua_setfield(L, -2, "__newindex");

        lua_pushcfunction(L, &lua_binder::instance_gc);
        lua_setfield(L, -2, "__gc");

        // store in registry: registry[type_details*] = metatable
        lua_settable(L, LUA_REGISTRYINDEX);
    }

    void* lua_binder::instance_object_ptr(lua_instance_header* header)
    {
        void* data_area =
            reinterpret_cast<char*>(header) + sizeof(lua_instance_header);
        switch (header->ownership) {
        case lua_instance_ownership::FOREIGN_POINTER:
            return *reinterpret_cast<void**>(data_area);
        case lua_instance_ownership::SHARED_PTR:
            return reinterpret_cast<std::shared_ptr<void>*>(data_area)->get();
        default:
            return data_area;
        }
    }

    namespace {
        bool is_lua_compatible(lua_State*                              L,
                               int                                     idx,
                               const mge::reflection::type_identifier& tid)
        {
            auto ti = tid.type_index();
            auto lua_type_at = lua_type(L, idx);

            // integer types
            if (ti == std::type_index(typeid(int8_t)) ||
                ti == std::type_index(typeid(uint8_t)) ||
                ti == std::type_index(typeid(int16_t)) ||
                ti == std::type_index(typeid(uint16_t)) ||
                ti == std::type_index(typeid(int32_t)) ||
                ti == std::type_index(typeid(uint32_t)) ||
                ti == std::type_index(typeid(int64_t)) ||
                ti == std::type_index(typeid(uint64_t))) {
                return lua_type_at == LUA_TNUMBER;
            }
            // float types
            if (ti == std::type_index(typeid(float)) ||
                ti == std::type_index(typeid(double)) ||
                ti == std::type_index(typeid(long double))) {
                return lua_type_at == LUA_TNUMBER;
            }
            // bool
            if (ti == std::type_index(typeid(bool))) {
                return lua_type_at == LUA_TBOOLEAN;
            }
            // string
            if (ti == std::type_index(typeid(std::string_view)) ||
                ti == std::type_index(typeid(std::string))) {
                return lua_type_at == LUA_TSTRING;
            }
            // enum types are stored as integers in Lua
            const auto& type_ref = mge::reflection::type_details::get(tid);
            if (type_ref && type_ref->is_enum) {
                return lua_type_at == LUA_TNUMBER;
            }
            // userdata (pointer or class)
            return lua_type_at == LUA_TUSERDATA;
        }
    } // namespace

    int lua_binder::class_call(lua_State* L)
    {
        // stack: [1] = class table, [2..n] = constructor args
        // get type_details from class table
        lua_getfield(L, 1, "__details__");
        auto* details = static_cast<const mge::reflection::type_details*>(
            lua_touserdata(L, -1));
        lua_pop(L, 1);

        if (!details || !details->is_class) {
            return luaL_error(L, "invalid class table");
        }

        const auto& class_details =
            std::get<mge::reflection::type_details::class_specific_details>(
                details->specific_details);

        int nargs = lua_gettop(L) - 1; // exclude class table

        // find matching constructor
        const mge::reflection::invoke_function* matched_ctor = nullptr;
        for (const auto& [sig, invoke_fn] : class_details.constructors) {
            const auto& params = sig.parameter_types();
            if (static_cast<int>(params.size()) != nargs) {
                continue;
            }
            bool compatible = true;
            for (int i = 0; i < nargs; ++i) {
                if (!is_lua_compatible(L, i + 2, params[i])) {
                    compatible = false;
                    break;
                }
            }
            if (compatible) {
                matched_ctor = &invoke_fn;
                break;
            }
        }

        if (!matched_ctor) {
            return luaL_error(L,
                              "no matching constructor for '%s' with %d args",
                              std::string(details->name).c_str(),
                              nargs);
        }

        // allocate userdata
        size_t ud_size = sizeof(lua_instance_header) + details->size;
        auto*  header =
            static_cast<lua_instance_header*>(lua_newuserdata(L, ud_size));
        header->type = details;
        header->ownership = lua_instance_ownership::OWNED;

        void* obj_ptr = instance_object_ptr(header);

        // invoke constructor
        lua_call_context ctx(L, 2, obj_ptr);
        (*matched_ctor)(ctx);

        // set instance metatable from registry
        lua_pushlightuserdata(
            L,
            const_cast<mge::reflection::type_details*>(details));
        lua_gettable(L, LUA_REGISTRYINDEX);
        lua_setmetatable(L, -2);

        return 1;
    }

    int lua_binder::instance_index(lua_State* L)
    {
        // stack: [1] = userdata, [2] = key (string)
        auto* header = static_cast<lua_instance_header*>(lua_touserdata(L, 1));
        if (!header || !header->type) {
            return luaL_error(L, "invalid instance");
        }

        const char* key = luaL_checkstring(L, 2);
        void*       obj_ptr = instance_object_ptr(header);

        const auto& class_details =
            std::get<mge::reflection::type_details::class_specific_details>(
                header->type->specific_details);

        for (const auto& [name, type_id, getter, setter] :
             class_details.fields) {
            if (name == key) {
                lua_call_context ctx(L, 0, obj_ptr);
                getter(ctx);
                return ctx.num_results();
            }
        }

        for (const auto& [name, sig, invoke_fn, is_const, is_noexcept] :
             class_details.methods) {
            if (name == key) {
                // push userdata as upvalue 1, method name as upvalue 2
                lua_pushvalue(L, 1);
                lua_pushstring(L, key);
                lua_pushcclosure(L, &lua_binder::method_call, 2);
                return 1;
            }
        }

        return luaL_error(L,
                          "'%s' has no field or method '%s'",
                          std::string(header->type->name).c_str(),
                          key);
    }

    int lua_binder::method_call(lua_State* L)
    {
        // upvalue 1 = userdata (self), upvalue 2 = method name
        auto* header = static_cast<lua_instance_header*>(
            lua_touserdata(L, lua_upvalueindex(1)));
        if (!header || !header->type) {
            return luaL_error(L, "invalid instance in method call");
        }

        const char* method_name = lua_tostring(L, lua_upvalueindex(2));
        void*       obj_ptr = instance_object_ptr(header);

        const auto& class_details =
            std::get<mge::reflection::type_details::class_specific_details>(
                header->type->specific_details);

        int nargs = lua_gettop(L) - 1; // exclude self (arg 1 from : syntax)

        for (const auto& [name, sig, invoke_fn, is_const, is_noexcept] :
             class_details.methods) {
            if (name != method_name) {
                continue;
            }
            const auto& params = sig.parameter_types();
            if (static_cast<int>(params.size()) != nargs) {
                continue;
            }
            bool compatible = true;
            for (int i = 0; i < nargs; ++i) {
                if (!is_lua_compatible(L, i + 2, params[i])) {
                    compatible = false;
                    break;
                }
            }
            if (compatible) {
                // Determine if return type is pointer/shared_ptr to class
                const mge::reflection::type_details* pointer_element_type =
                    nullptr;
                const mge::reflection::type_details* shared_ptr_element_type =
                    nullptr;
                const auto& return_type_id = sig.return_type();
                const auto& return_type_details =
                    mge::reflection::type_details::get(return_type_id);
                if (return_type_details && return_type_details->is_pointer) {
                    const auto& ptr_details =
                        std::get<mge::reflection::type_details::
                                     pointer_specific_details>(
                            return_type_details->specific_details);
                    if (ptr_details.element_type &&
                        ptr_details.element_type->is_class) {
                        pointer_element_type = ptr_details.element_type.get();
                    }
                } else if (return_type_details &&
                           return_type_details->is_class) {
                    const auto& ret_class = std::get<
                        mge::reflection::type_details::class_specific_details>(
                        return_type_details->specific_details);
                    if (ret_class.is_shared_ptr &&
                        ret_class.shared_ptr_element_type) {
                        shared_ptr_element_type =
                            ret_class.shared_ptr_element_type.get();
                    }
                }

                lua_call_context ctx(L, 2, obj_ptr);
                if (pointer_element_type) {
                    ctx.set_pointer_result_type(pointer_element_type);
                }
                if (shared_ptr_element_type) {
                    ctx.set_shared_ptr_result_type(shared_ptr_element_type);
                }
                invoke_fn(ctx);
                return ctx.num_results();
            }
        }

        return luaL_error(L,
                          "no matching method '%s' on '%s' with %d args",
                          method_name,
                          std::string(header->type->name).c_str(),
                          nargs);
    }

    int lua_binder::instance_newindex(lua_State* L)
    {
        // stack: [1] = userdata, [2] = key (string), [3] = value
        auto* header = static_cast<lua_instance_header*>(lua_touserdata(L, 1));
        if (!header || !header->type) {
            return luaL_error(L, "invalid instance");
        }

        const char* key = luaL_checkstring(L, 2);
        void*       obj_ptr = instance_object_ptr(header);

        const auto& class_details =
            std::get<mge::reflection::type_details::class_specific_details>(
                header->type->specific_details);

        for (const auto& [name, type_id, getter, setter] :
             class_details.fields) {
            if (name == key) {
                if (!setter) {
                    return luaL_error(L, "field '%s' is read-only", key);
                }
                lua_call_context ctx(L, 3, obj_ptr);
                setter(ctx);
                return 0;
            }
        }

        return luaL_error(L,
                          "'%s' has no field '%s'",
                          std::string(header->type->name).c_str(),
                          key);
    }

    int lua_binder::instance_gc(lua_State* L)
    {
        auto* header = static_cast<lua_instance_header*>(lua_touserdata(L, 1));
        if (!header || !header->type) {
            return 0;
        }

        switch (header->ownership) {
        case lua_instance_ownership::FOREIGN_POINTER:
            return 0;
        case lua_instance_ownership::SHARED_PTR: {
            void* data_area =
                reinterpret_cast<char*>(header) + sizeof(lua_instance_header);
            reinterpret_cast<std::shared_ptr<void>*>(data_area)->~shared_ptr();
            return 0;
        }
        default:
            break;
        }

        const auto& class_details =
            std::get<mge::reflection::type_details::class_specific_details>(
                header->type->specific_details);

        if (class_details.destructor) {
            void*            obj_ptr = instance_object_ptr(header);
            lua_call_context ctx(L, 0, obj_ptr);
            class_details.destructor(ctx);
        }

        return 0;
    }

    void lua_binder::create_foreign_instance(
        lua_State*                           L,
        const mge::reflection::type_details* target_type,
        void*                                ptr)
    {
        size_t ud_size = sizeof(lua_instance_header) + sizeof(void*);
        auto*  header =
            static_cast<lua_instance_header*>(lua_newuserdata(L, ud_size));
        header->type = target_type;
        header->ownership = lua_instance_ownership::FOREIGN_POINTER;
        void** data_area = reinterpret_cast<void**>(
            reinterpret_cast<char*>(header) + sizeof(lua_instance_header));
        *data_area = ptr;

        // set instance metatable from registry
        lua_pushlightuserdata(
            L,
            const_cast<mge::reflection::type_details*>(target_type));
        lua_gettable(L, LUA_REGISTRYINDEX);
        lua_setmetatable(L, -2);
    }

    void lua_binder::create_shared_instance(
        lua_State*                           L,
        const mge::reflection::type_details* element_type,
        std::shared_ptr<void>                ptr)
    {
        size_t ud_size =
            sizeof(lua_instance_header) + sizeof(std::shared_ptr<void>);
        auto* header =
            static_cast<lua_instance_header*>(lua_newuserdata(L, ud_size));
        header->type = element_type;
        header->ownership = lua_instance_ownership::SHARED_PTR;
        void* data_area =
            reinterpret_cast<char*>(header) + sizeof(lua_instance_header);
        new (data_area) std::shared_ptr<void>(std::move(ptr));

        // set instance metatable from registry
        lua_pushlightuserdata(
            L,
            const_cast<mge::reflection::type_details*>(element_type));
        lua_gettable(L, LUA_REGISTRYINDEX);
        lua_setmetatable(L, -2);
    }

    int lua_binder::static_method_call(lua_State* L)
    {
        // upvalue 1 = type_details*, upvalue 2 = method index
        auto* details = static_cast<const mge::reflection::type_details*>(
            lua_touserdata(L, lua_upvalueindex(1)));
        auto method_index =
            static_cast<size_t>(lua_tointeger(L, lua_upvalueindex(2)));

        if (!details || !details->is_class) {
            return luaL_error(L, "invalid class in static method call");
        }

        const auto& class_details =
            std::get<mge::reflection::type_details::class_specific_details>(
                details->specific_details);

        if (method_index >= class_details.static_methods.size()) {
            return luaL_error(L, "invalid static method index");
        }

        const auto& [name, sig, invoke_fn] =
            class_details.static_methods[method_index];

        int         nargs = lua_gettop(L);
        const auto& params = sig.parameter_types();
        if (static_cast<int>(params.size()) != nargs) {
            return luaL_error(L,
                              "static method '%s' expects %d args, got %d",
                              std::string(name).c_str(),
                              static_cast<int>(params.size()),
                              nargs);
        }

        for (int i = 0; i < nargs; ++i) {
            if (!is_lua_compatible(L, i + 1, params[i])) {
                return luaL_error(
                    L,
                    "incompatible argument %d for static method '%s'",
                    i + 1,
                    std::string(name).c_str());
            }
        }

        // Determine if return type is pointer to class
        const mge::reflection::type_details* pointer_element_type = nullptr;
        const mge::reflection::type_details* shared_ptr_element_type = nullptr;
        const auto&                          return_type_id = sig.return_type();
        const auto&                          return_type_details =
            mge::reflection::type_details::get(return_type_id);
        if (return_type_details && return_type_details->is_pointer) {
            const auto& ptr_details = std::get<
                mge::reflection::type_details::pointer_specific_details>(
                return_type_details->specific_details);
            if (ptr_details.element_type &&
                ptr_details.element_type->is_class) {
                pointer_element_type = ptr_details.element_type.get();
            }
        } else if (return_type_details && return_type_details->is_class) {
            const auto& ret_class =
                std::get<mge::reflection::type_details::class_specific_details>(
                    return_type_details->specific_details);
            if (ret_class.is_shared_ptr && ret_class.shared_ptr_element_type) {
                shared_ptr_element_type =
                    ret_class.shared_ptr_element_type.get();
            }
        }

        lua_call_context ctx(L, 1, nullptr);
        if (pointer_element_type) {
            ctx.set_pointer_result_type(pointer_element_type);
        }
        if (shared_ptr_element_type) {
            ctx.set_shared_ptr_result_type(shared_ptr_element_type);
        }
        invoke_fn(ctx);
        return ctx.num_results();
    }

    void lua_binder::after(const mge::reflection::type_details& details) {}
    void lua_binder::before(const mge::reflection::function_details& details) {}
    void lua_binder::on(const mge::reflection::function_details& details) {}
    void lua_binder::after(const mge::reflection::function_details& details) {}
} // namespace mge::lua