#include "lua_type.hpp"
#include "lua_context.hpp"
#include "lua_error.hpp"
#include "value_classification.hpp"

#include "mge/core/checked_cast.hpp"
#include "mge/core/gist.hpp"
#include "mge/core/trace.hpp"

namespace mge {
    MGE_USE_TRACE(LUA);
}

namespace mge::lua {

    type::type(lua_context&                         context,
               const mge::script::type_details_ref& details)
        : m_context(context)
        , m_details(details)
        , m_materialized(false)
        , m_ctor_defined(false)
        , m_delete_ptr(nullptr)
        , m_delete_shared_ptr(nullptr)
    {}

    type::~type() {}

    void type::add_enum_value(const std::string& name, int64_t enum_value)
    {
        MGE_DEBUG_TRACE(LUA) << "Add enum " << name << " = " << enum_value
                             << " to type " << m_details->name();

        auto        L = m_context.lua_state();
        lua_Integer lua_value = checked_cast<lua_Integer>(enum_value);
        lua_pushinteger(L, lua_value);
        CHECK_CURRENT_STATUS(L);
        lua_setfield(L, -2, name.c_str());
        CHECK_CURRENT_STATUS(L);
    }

    void type::add_to_parent(lua_context& context)
    {
        auto L = context.lua_state();
        // create meta table for type
        // use stable type details pointer as key
        lua_pushlightuserdata(L, m_details.get());
        lua_newtable(L);
        lua_settable(L, LUA_REGISTRYINDEX);
        // create type table
        lua_newtable(L);
        // -1 - type table
        // -2 - module table or enclosing type
        CHECK_CURRENT_STATUS(L);
        lua_pushnil(L);
        // -3 parent table
        // -2 type table
        // -1 nil
        lua_copy(L, -2, -1);
        // -3 parent table
        // -2 type table
        // -1 type table
        lua_setfield(L, -3, m_details->name().c_str());
        CHECK_CURRENT_STATUS(L);
        // leave parent table on stack
        // leave type table on stack
        m_materialized = true;
        return;
    }

    void type::add_type(const type_ref& t)
    {
        if (!m_types.has_value()) {
            m_types = std::vector<type_ref>({t});
        } else {
            m_types->emplace_back(t);
        }
    }

    void type::push_type_table()
    {
        if (!m_materialized) {
            MGE_THROW(mge::illegal_state) << "Type '" << m_details->name()
                                          << "' is not registered in LUA";
        }
        auto L = m_context.lua_state();
        if (!lua_istable(L, -1)) {
            MGE_THROW(mge::illegal_state) << "No table found at top of stack";
        }
        auto rc = lua_getfield(L, -1, m_details->name().c_str());
        CHECK_TYPE(LUA_TTABLE, rc);
    }

    void type::pop_type_table()
    {
        auto L = m_context.lua_state();
        if (!lua_istable(L, -1)) {
            MGE_THROW(mge::illegal_state) << "Type table not on top of stack";
        }
        lua_pop(L, 1);
        CHECK_CURRENT_STATUS(L);
    }

    void type::add_constructor(const mge::script::signature&       s,
                               const mge::script::invoke_function& new_at,
                               const mge::script::invoke_function& new_shared)
    {
        MGE_DEBUG_TRACE(LUA) << "Add constructor for '" << m_details->name()
                             << "' " << mge::gist(s);

        m_constructors[s.size()].emplace_back(&s, &new_at, &new_shared);
        if (!m_ctor_defined && m_delete_shared_ptr && m_delete_ptr) {
            define_construction();
        }
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

        MGE_DEBUG_TRACE(LUA)
            << "Destruct value of type '" << self->m_details->name()
            << "' with " << top << " arguments";

        // void* shared_ptr_address = lua_touserdata(L, 1);
        return 0;
    }

    void
    type::set_destructor(const mge::script::invoke_function& delete_ptr,
                         const mge::script::invoke_function& delete_shared_ptr)
    {
        m_delete_ptr = &delete_ptr;
        m_delete_shared_ptr = &delete_shared_ptr;
        if (!m_ctor_defined && m_delete_shared_ptr && m_delete_ptr &&
            !m_constructors.empty()) {
            define_construction();
        }
        auto L = m_context.lua_state();
        load_metatable(L);
        lua_pushstring(L, "__gc");
        lua_pushlightuserdata(L, this);
        lua_pushcclosure(L, &destruct, 1);
        // stack now
        // -1 closure
        // -2 string "__gc"
        // -3 meta table
        lua_settable(L, -3);
        // meta table remains on stack, remove
        lua_pop(L, 1);
    }

    void type::load_metatable(lua_State* L)
    {
        lua_pushlightuserdata(L, m_details.get());
        lua_gettable(L, LUA_REGISTRYINDEX);
        if (!lua_istable(L, -1)) {
            lua_pushfstring(L,
                            "Meta table for type %s not found",
                            m_details->name().c_str());
            lua_error(L);
        }
    }

    int type::construct(lua_State* L)
    {
        int top = lua_gettop(L);

        void* self_ptr = lua_touserdata(L, lua_upvalueindex(1));
        type* self = reinterpret_cast<type*>(self_ptr);
        MGE_DEBUG_TRACE(LUA)
            << "Construct value of type '" << self->m_details->name()
            << "' with " << (top - 1) << " arguments";

        const constructor* ctor = self->select_constructor(top - 1, L);
        if (ctor) {
            void* shared_ptr_mem = lua_newuserdata(L, sizeof(void*));
            memset(shared_ptr_mem, 0, sizeof(void*));
            self->load_metatable(L);
            // stack now
            // -1 - meta table
            // -2 - new user data (still nullptr)
            lua_setmetatable(L, -2);
        } else {
            lua_pushfstring(L,
                            "Cannot construct object of type '%s'",
                            self->m_details->name().c_str());
            lua_error(L);
        }
        return 1;
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
                    auto match = value_classes[i].match(all_ctors[ci].s->at(i));
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

    void type::define_construction()
    {
        MGE_DEBUG_TRACE(LUA)
            << "Define construction for type '" << m_details->name() << "'";
        // on stack we have the table of this type X
        // reachable globally at module.X
        auto L = m_context.lua_state();
        lua_pushlightuserdata(L, this);
        lua_pushcclosure(L, construct, 1);
        lua_setfield(L, -2, "new");
        CHECK_CURRENT_STATUS(L);
    }

} // namespace mge::lua