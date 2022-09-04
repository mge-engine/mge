#include "lua_type.hpp"
#include "lua_context.hpp"
#include "lua_error.hpp"

namespace mge::lua {

    type::type(lua_context&                         context,
               const mge::script::type_details_ref& details)
        : m_create_data(std::make_unique<create_data>())
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
    }

} // namespace mge::lua