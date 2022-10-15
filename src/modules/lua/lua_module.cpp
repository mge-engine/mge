#include "lua_module.hpp"
#include "mge/script/module_details.hpp"

namespace mge::lua {
    module::module(lua_context&                           context,
                   const mge::script::module_details_ref& m)
        : scope(context, m->name().c_str())
        , m_module(m)
    {}

    module::module(lua_context&                           context,
                   const scope_ref&                       parent,
                   const mge::script::module_details_ref& m)
        : scope(context, parent, m->name().c_str())
        , m_module(m)
    {}

    module::~module() {}

} // namespace mge::lua