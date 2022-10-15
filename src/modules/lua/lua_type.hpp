#pragma once
#include "lua_scope.hpp"

namespace mge::lua {

    class type : public scope
    {
    public:
        type(lua_context&                         context,
             const scope_ref&                     parent,
             const mge::script::type_details_ref& t);
        virtual ~type() = default;

    private:
        mge::script::type_details_ref m_details;
    };

} // namespace mge::lua