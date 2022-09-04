#include "lua_type.hpp"

namespace mge::lua {

    type::type(lua_context&                         context,
               const mge::script::type_details_ref& details)
        : m_create_data(std::make_unique<create_data>())
        , m_details(details)
    {}

    type::~type() {}

    void type::add_enum_value(const std::string& name, int64_t enum_value) {}

} // namespace mge::lua