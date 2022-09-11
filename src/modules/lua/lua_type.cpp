#include "lua_type.hpp"
#include "lua_context.hpp"
#include "lua_error.hpp"

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
        m_constructors.emplace_back(&s, &new_at, &new_shared);
    }

    void
    type::set_destructor(const mge::script::invoke_function& delete_ptr,
                         const mge::script::invoke_function& delete_shared_ptr)
    {
        m_delete_ptr = &delete_ptr;
        m_delete_shared_ptr = &delete_shared_ptr;
    }

} // namespace mge::lua