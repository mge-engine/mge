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
        int argc = lua_gettop(L);

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
            lua_object_call_context ctx(self, L, shared_ptr_mem);
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
        if (lua_type(L, lua_upvalueindex(1)) != LUA_TLIGHTUSERDATA) {
            return 0;
        }
        void* self_ptr = lua_touserdata(L, lua_upvalueindex(1));
        type* self = reinterpret_cast<type*>(self_ptr);

        void*                   shared_ptr_address = lua_touserdata(L, 1);
        lua_object_call_context ctx(self, L, shared_ptr_address);
        (*self->m_delete_shared_ptr)(ctx);

        return 0;
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