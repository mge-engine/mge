#include "lua_type.hpp"

namespace mge::lua {
    type::type(lua_context& context, const mge::script::type_data_ref& t)
        : m_context(context)
        , m_type(t)
    {}

    lua_type::~lua_type() {}
} // namespace mge::lua