#include "lua_type.hpp"

#include "mge/core/details.hpp"
#include "mge/core/small_vector.hpp"
#include "mge/core/trace.hpp"

#include "mge/script/type_details.hpp"

#include "lua_object_call_context.hpp"
#include "value_classification.hpp"

namespace mge {
    MGE_USE_TRACE(LUA);
}

namespace mge::lua {
    type::type(lua_context&                         context,
               const scope_ref&                     parent,
               const mge::script::type_details_ref& t)
        : scope(context, parent, t->name().c_str())
        , m_details(t)
        , m_delete_ptr(nullptr)
        , m_delete_shared_ptr(nullptr)
    {
        create_instance_metatable();
        create_type_metatable();
        define_construction();
    }

    void type::create_instance_metatable()
    {
        auto L = m_context.lua_state();
        lua_pushlightuserdata(L, this);
        lua_newtable(L);

        lua_pushstring(L, "__gc");
        lua_pushlightuserdata(L, this);
        lua_pushcclosure(L, &destruct, 1);
        lua_settable(L, -3);

        lua_pushstring(L, "__index");
        lua_pushlightuserdata(L, this);
        lua_pushcclosure(L, &index, 1);
        lua_settable(L, -3);

        lua_settable(L, LUA_REGISTRYINDEX);
    }

    void type::load_instance_metatable()
    {
        auto L = m_context.lua_state();
        lua_pushlightuserdata(L, this);
        lua_gettable(L, LUA_REGISTRYINDEX);
    }

    void type::create_type_metatable()
    {
        auto L = m_context.lua_state();
        lua_pushlightuserdata(L, const_cast<char*>(this->m_name));
        lua_newtable(L);
        lua_settable(L, LUA_REGISTRYINDEX);

        load();
        lua_pushlightuserdata(L, const_cast<char*>(this->m_name));
        lua_gettable(L, LUA_REGISTRYINDEX);
        lua_setmetatable(L, -2);
        lua_pop(L, 1);
    }

    void type::define_construction()
    {
        auto L = m_context.lua_state();
        lua_pushlightuserdata(L, const_cast<char*>(this->m_name));
        lua_gettable(L, LUA_REGISTRYINDEX);
        lua_pushlightuserdata(L, this);
        lua_pushcclosure(L, &construct, 1);
        lua_setfield(L, -2, "__call");
        lua_pop(L, 1);
    }

    int type::construct(lua_State* L)
    {
        int argc = lua_gettop(L) - 1;

        void* self_ptr = lua_touserdata(L, lua_upvalueindex(1));
        type* self = reinterpret_cast<type*>(self_ptr);

        const constructor* ctor = self->select_constructor(argc, L);
        if (ctor) {
            void* shared_ptr_mem = lua_newuserdata(L, sizeof(void*));
            memset(shared_ptr_mem, 0, sizeof(void*));
            self->load_instance_metatable();
            // stack now
            // -1 - meta table
            // -2 - new user data (still nullptr)
            lua_setmetatable(L, -2);
            lua_object_call_context ctx(self, L, shared_ptr_mem, 1);
            (*ctor->new_shared)(ctx);
        } else {
            lua_pushfstring(L,
                            "Cannot construct object of type '%s'",
                            self->m_details->name().c_str());
            lua_error(L);
        }
        return 1;
    }

    int type::destruct(lua_State* L)
    {
        int top = lua_gettop(L);
        if (top != 1) {
            return 0;
        }
        void* self_ptr = lua_touserdata(L, lua_upvalueindex(1));
        if (!self_ptr) {
            return 0;
        }
        type* self = reinterpret_cast<type*>(self_ptr);

        void* shared_ptr_address = lua_touserdata(L, 1);
        if (!shared_ptr_address) {
            return 0;
        }
        lua_object_call_context ctx(self, L, shared_ptr_address);
        (*self->m_delete_shared_ptr)(ctx);

        return 0;
    }

    int type::method_index(const char* name) const
    {
        auto it = m_methods.find(name);
        if (it == m_methods.end()) {
            return 0;
        }
        auto L = m_context.lua_state();
        lua_pushlightuserdata(L, const_cast<type*>(this));
        lua_pushlightuserdata(L, const_cast<char*>(it->first));
        lua_pushlightuserdata(L, const_cast<std::vector<method>*>(&it->second));
        lua_pushcclosure(L, &call_method, 3);
        return 1;
    }

    int type::static_method_index(const char* name) const
    {
        auto it = m_static_methods.find(name);
        if (it == m_static_methods.end()) {
            return 0;
        }
        auto L = m_context.lua_state();
        lua_pushlightuserdata(L, const_cast<type*>(this));
        lua_pushlightuserdata(L, const_cast<char*>(it->first));
        lua_pushlightuserdata(L, const_cast<std::vector<method>*>(&it->second));
        lua_pushcclosure(L, &call_static_method, 3);
        return 1;
    }

    int type::index(lua_State* L)
    {
        int top = lua_gettop(L);
        if (top != 2) {
            return 0;
        }
        if (lua_type(L, lua_upvalueindex(1)) != LUA_TLIGHTUSERDATA) {
            return 0;
        }

        void*       self_ptr = lua_touserdata(L, lua_upvalueindex(1));
        type*       self = reinterpret_cast<type*>(self_ptr);
        const char* name = lua_tostring(L, 2);

        auto it = self->m_fields.find(name);
        if (it == self->m_fields.end()) {
            int rc = self->method_index(name);
            if (rc != 0) {
                return rc;
            }

            rc = self->static_method_index(name);
            if (rc != 0) {
                return rc;
            }

            lua_pushfstring(L,
                            "Type %s has no field %s",
                            self->m_details->name().c_str(),
                            name);
            lua_error(L);
        }
        void* shared_ptr_address = lua_touserdata(L, 1);

        lua_object_call_context ctx(self, L, shared_ptr_address);
        (*it->second.getter)(ctx);
        return 1;
    }

    int type::call_method(lua_State* L)
    {
        for (int i = 1; i <= 3; ++i) {
            if (lua_type(L, lua_upvalueindex(i)) != LUA_TLIGHTUSERDATA) {
                return 0;
            }
        }

        type* self =
            reinterpret_cast<type*>(lua_touserdata(L, lua_upvalueindex(1)));
        const char* name = reinterpret_cast<const char*>(
            lua_touserdata(L, lua_upvalueindex(2)));

        std::vector<method>* methods = reinterpret_cast<std::vector<method>*>(
            lua_touserdata(L, lua_upvalueindex(3)));
        if (!self || !name || !methods || methods->empty()) {
            return 0;
        }
        int top = lua_gettop(L);
        if (top < 1) {
            lua_pushfstring(L,
                            "Cannot call method %s.%s with no object",
                            self->m_details->name().c_str(),
                            name);
            lua_error(L);
        }

        void* shared_ptr_address = lua_touserdata(L, 1);
        if (!shared_ptr_address) {
            lua_pushfstring(
                L,
                "Cannot call method %s.%s with nil object reference",
                self->m_details->name().c_str(),
                name);
            lua_error(L);
        }

        const mge::script::invoke_function* invoke = nullptr;
        size_t                              best_match_count = 0;

        if (methods->size() > 1) {
            mge::small_vector<value_classification, 3> value_classes;
            for (int i = 2; i <= top; ++i) {
                value_classes.push_back(value_classification(L, i));
            }
            for (size_t i = 0; i < methods->size(); ++i) {
                size_t exact_match_count = 0;
                bool   match_failed = false;
                for (size_t j = 0; j < value_classes.size(); ++j) {
                    auto match =
                        value_classes[j].match(methods->at(i).signature->at(j));
                    if (match == value_classification::NO_MATCH) {
                        match_failed = true;
                        break;
                    } else if (match == value_classification::MATCH_EXACT) {
                        ++exact_match_count;
                    }
                }

                if (!match_failed) {
                    if (exact_match_count > best_match_count) {
                        invoke = (*methods)[i].invoke;
                        best_match_count = exact_match_count;
                    } else if (invoke == nullptr) {
                        invoke = (*methods)[i].invoke;
                    }
                }
            }
        } else {
            invoke = (*methods)[0].invoke;
        }

        lua_object_call_context ctx(self, L, shared_ptr_address, 1);
        (*invoke)(ctx);

        return 1;
    }

    int type::call_static_method(lua_State* L)
    {
        for (int i = 1; i <= 3; ++i) {
            if (lua_type(L, lua_upvalueindex(i)) != LUA_TLIGHTUSERDATA) {
                return 0;
            }
        }

        type* self =
            reinterpret_cast<type*>(lua_touserdata(L, lua_upvalueindex(1)));
        const char* name = reinterpret_cast<const char*>(
            lua_touserdata(L, lua_upvalueindex(1)));

        std::vector<method>* methods = reinterpret_cast<std::vector<method>*>(
            lua_touserdata(L, lua_upvalueindex(2)));
        if (!name || !methods || methods->empty()) {
            return 0;
        }
        int top = lua_gettop(L);

        const mge::script::invoke_function* invoke = nullptr;
        size_t                              best_match_count = 0;

        if (methods->size() > 1) {
            mge::small_vector<value_classification, 3> value_classes;
            for (int i = 1; i <= top; ++i) {
                value_classes.push_back(value_classification(L, i));
            }
            for (size_t i = 0; i < methods->size(); ++i) {
                size_t exact_match_count = 0;
                bool   match_failed = false;
                for (size_t j = 0; j < value_classes.size(); ++j) {
                    auto match =
                        value_classes[j].match(methods->at(i).signature->at(j));
                    if (match == value_classification::NO_MATCH) {
                        match_failed = true;
                        break;
                    } else if (match == value_classification::MATCH_EXACT) {
                        ++exact_match_count;
                    }
                }

                if (!match_failed) {
                    if (exact_match_count > best_match_count) {
                        invoke = (*methods)[i].invoke;
                        best_match_count = exact_match_count;
                    } else if (invoke == nullptr) {
                        invoke = (*methods)[i].invoke;
                    }
                }
            }
        } else {
            invoke = (*methods)[0].invoke;
        }

        lua_object_call_context ctx(self, L, nullptr);
        (*invoke)(ctx);

        return 1;
    }

    void type::add_constructor(const mge::script::signature&       signature,
                               const mge::script::invoke_function& new_at,
                               const mge::script::invoke_function& new_shared)
    {
        constructor ctor{&signature, &new_at, &new_shared};
        m_constructors[signature.size()].push_back(ctor);
    }

    void type::add_field(const std::string&                   name,
                         const mge::script::type_details_ref& type,
                         const mge::script::invoke_function&  setter,
                         const mge::script::invoke_function&  getter)
    {
        field f{&name, &type, &setter, &getter};
        m_fields[name.c_str()] = f;
    }

    void type::add_method(const std::string&                  name,
                          const std::type_index&              return_type,
                          const mge::script::signature&       sig,
                          const mge::script::invoke_function& invoke)
    {
        method m{&return_type, &sig, &invoke};
        m_methods[name.c_str()].push_back(m);
    }

    void type::add_static_method(const std::string&            name,
                                 const std::type_index&        return_type,
                                 const mge::script::signature& sig,
                                 const mge::script::invoke_function& invoke)
    {
        method m{&return_type, &sig, &invoke};
        m_static_methods[name.c_str()].push_back(m);
    }

    void
    type::set_destructor(const mge::script::invoke_function& delete_ptr,
                         const mge::script::invoke_function& delete_shared_ptr)
    {
        m_delete_ptr = &delete_ptr;
        m_delete_shared_ptr = &delete_shared_ptr;
    }

    const type::constructor* type::select_constructor(int        nargs,
                                                      lua_State* L) const
    {
        // MGE_DEBUG_TRACE(LUA) << "construct with " << nargs << " args";
        auto it = m_constructors.find(static_cast<size_t>(nargs));
        if (it != m_constructors.end()) {
            if (it->second.size() == 1) {
                return &((it->second)[0]);
            }

            mge::small_vector<value_classification, 3> value_classes;
            for (int i = 1; i <= nargs; ++i) {
                value_classes.push_back(value_classification(L, i));
            }
            const auto& all_ctors = it->second;
            const auto  all_ctors_size = all_ctors.size();
            size_t      best_constructor = all_ctors_size;
            size_t      best_constructor_match_count = 0;

            for (size_t ci = 0; ci < all_ctors_size; ++ci) {

                size_t exact_match_count = 0;
                bool   match_failed = false;

                for (int i = 0; i < nargs; ++i) {
                    auto match =
                        value_classes[i].match(all_ctors[ci].signature->at(i));
                    if (match == value_classification::NO_MATCH) {
                        match_failed = true;
                        break;
                    } else if (match == value_classification::MATCH_EXACT) {
                        ++exact_match_count;
                    }
                }

                if (!match_failed) {
                    if (exact_match_count > best_constructor_match_count) {
                        best_constructor = ci;
                    } else if (best_constructor == all_ctors_size) {
                        best_constructor = ci;
                    }
                }
            }

            if (best_constructor != all_ctors_size) {
                return &(all_ctors[best_constructor]);
            }
        }
        return nullptr;
    }

} // namespace mge::lua