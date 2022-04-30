#include "lua_module.hpp"
#include "lua_context.hpp"

namespace mge::lua {
    lua_module::lua_module(lua_context& context, const mge::script::module& m)
        : m_context(context)
        , m_module(m)
    {}

} // namespace mge::lua