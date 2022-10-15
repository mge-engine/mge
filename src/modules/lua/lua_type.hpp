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

        static int construct(lua_State* L);
        static int destruct(lua_State* L);

        const mge::script::type_details_ref& details() const noexcept
        {
            return m_details;
        }

    private:
        void create_instance_metatable();
        void create_type_metatable();
        void define_construction();

        mge::script::type_details_ref m_details;
    };

} // namespace mge::lua