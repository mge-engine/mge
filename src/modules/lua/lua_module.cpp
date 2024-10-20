#include "lua_module.hpp"
#include "mge/script/module_data.hpp"

namespace mge::lua {
    module::module(lua_context& context, const char* name) :scope(context, name)
    {}

    // clang-format off
    module::module(lua_context&                        context,
                   const mge::script::module_data_ref& m) 
        : scope(context, m->name())
        , m_module(m)
    {}
   
    module::module(lua_context&                        context,
                   const scope_ref&                    parent,
                   const mge::script::module_data_ref& m) 
        : scope(context, parent, m->name())
        , m_module(m)
    {}
    // clang-format on

} // namespace mge::lua