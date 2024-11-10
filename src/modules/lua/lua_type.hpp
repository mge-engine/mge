#pragma once
#include "lua_context.hpp"

namespace mge::lua {

    class type
    {
    public:
        type(lua_context& context, const mge::script::type_data_ref& t);
        virtual ~type() = default;

    private:
        lua_context&               m_context;
        mge::script::type_data_ref m_type;
    };

} // namespace mge::lua