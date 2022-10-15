#pragma once
#include "lua_scope.hpp"

namespace mge::lua {

    class type : public scope
    {
    private:
        struct constructor
        {
            const mge::script::signature*       signature;
            const mge::script::invoke_function* new_at;
            const mge::script::invoke_function* new_shared;
        };

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

        void add_constructor(const mge::script::signature&       signature,
                             const mge::script::invoke_function& new_at,
                             const mge::script::invoke_function& new_shared);

        const constructor* select_constructor(int nargs, lua_State* L) const;

    private:
        void create_instance_metatable();
        void create_type_metatable();
        void define_construction();

        mge::script::type_details_ref              m_details;
        std::map<size_t, std::vector<constructor>> m_constructors;
    };

} // namespace mge::lua