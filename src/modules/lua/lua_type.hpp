#pragma once
#include "lua_context.hpp"

namespace mge::lua {

    class type
    {
    public:
        type(lua_context& context, const mge::script::type_data_ref& t);
        virtual ~type() = default;

    private:
        void create_type_metatable();
        void set_type_in_module();

        lua_context&               m_context;
        std::string                m_name_in_module;
        std::string                m_module_name;
        mge::script::type_data_ref m_type;
    };

} // namespace mge::lua