#include "lua_type.hpp"

namespace mge::lua {

    lua_type::lua_type(lua_context&                         context,
                       const mge::script::type_details_ref& type)
        : m_type(type)
    {}

    lua_type::~lua_type() {}

} // namespace mge::lua