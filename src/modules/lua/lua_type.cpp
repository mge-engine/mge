#include "lua_type.hpp"

namespace mge::lua {

    type::type(lua_context&                         context,
               const mge::script::type_details_ref& details)
        : m_details(details)
    {}

    type::~type() {}

} // namespace mge::lua