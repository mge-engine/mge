#include "lua_type.hpp"
#include "lua_context.hpp"
#include "lua_error.hpp"

#include "mge/core/checked_cast.hpp"
#include "mge/core/trace.hpp"

namespace mge {
    MGE_USE_TRACE(LUA);
}

namespace mge::lua {

    type::type(lua_context&                         context,
               const mge::script::type_details_ref& details)
        : m_context(context)
        , m_create_data(std::make_unique<create_data>())
        , m_details(details)
    {}

    type::~type() {}

    void type::assert_create_data()
    {
        if (!m_create_data) {
            MGE_THROW(mge::illegal_state)
                << "Lua type creation not in progress";
        }
    }

    void type::add_enum_value(const std::string& name, int64_t enum_value)
    {
        assert_create_data();
        m_create_data->enum_values[name] = enum_value;
    }

    void type::add_to_module(lua_context& context)
    {
        assert_create_data();
        auto L = context.lua_state();
        materialize(context);
        // -1 - type table
        // -2 - module table
        CHECK_CURRENT_STATUS(L);
        lua_setfield(context.lua_state(), -2, m_details->name().c_str());
        CHECK_CURRENT_STATUS(L);
        // leave module table on stack
        m_create_data.reset();
        return;
    }

    void type::materialize(lua_context& context)
    {
        auto L = context.lua_state();
        lua_newtable(L);
        if (!m_create_data->enum_values.empty()) {
            for (const auto& [name, value] : m_create_data->enum_values) {
                MGE_DEBUG_TRACE(LUA) << "Add enum " << name << " = " << value
                                     << " to type " << m_details->name();
                lua_Integer lua_value = checked_cast<lua_Integer>(value);
                lua_pushinteger(L, lua_value);
                CHECK_CURRENT_STATUS(L);
                lua_setfield(L, -2, name.c_str());
                CHECK_CURRENT_STATUS(L);
            }
        }
    }

    void type::push_type_table()
    {
        auto L = m_context.lua_state();
        auto rc = lua_getfield(L, -1, m_details->name().c_str());
        CHECK_TYPE(LUA_TTABLE, rc);
    }

    void type::pop_type_table()
    {
        auto L = m_context.lua_state();
        lua_pop(L, 1);
        CHECK_CURRENT_STATUS(L);
    }

} // namespace mge::lua