#include "lua_type.hpp"
#include "mge/script/type_details.hpp"

namespace mge::lua {
    type::type(lua_context&                         context,
               const scope_ref&                     parent,
               const mge::script::type_details_ref& t)
        : scope(context, parent, t->name().c_str())
        , m_details(t)
    {}

} // namespace mge::lua