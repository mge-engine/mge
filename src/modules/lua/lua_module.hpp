#pragma once
#include "lua_scope.hpp"

namespace mge::lua {

    class module : public scope
    {
    public:
        module(lua_context& context, const mge::script::module_details_ref& m);
        module(lua_context&                           context,
               const scope_ref&                       parent,
               const mge::script::module_details_ref& m);
        virtual ~module();

    private:
        mge::script::module_details_ref m_module;
    };

} // namespace mge::lua