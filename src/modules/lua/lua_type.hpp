// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "lua_scope.hpp"

#include "mge/core/cstring.hpp"

#include <map>

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

        struct field
        {
            const std::string*                   name;
            const mge::script::type_details_ref* type;
            const mge::script::invoke_function*  getter;
            const mge::script::invoke_function*  setter;
        };

        struct method
        {
            const std::type_index*              return_type;
            const mge::script::signature*       signature;
            const mge::script::invoke_function* invoke;
        };

    public:
        type(lua_context&                         context,
             const scope_ref&                     parent,
             const mge::script::type_details_ref& t);
        virtual ~type() = default;

        static int construct(lua_State* L);
        static int destruct(lua_State* L);
        static int index(lua_State* L);
        static int static_index(lua_State* L);
        int        method_index(const char* name) const;
        int        static_method_index(const char* name) const;
        static int call_method(lua_State* L);
        static int call_static_method(lua_State* L);

        const mge::script::type_details_ref& details() const noexcept
        {
            return m_details;
        }

        void add_constructor(const mge::script::signature&       signature,
                             const mge::script::invoke_function& new_at,
                             const mge::script::invoke_function& new_shared);

        void add_field(const std::string&                   name,
                       const mge::script::type_details_ref& type,
                       const mge::script::invoke_function&  setter,
                       const mge::script::invoke_function&  getter);

        void add_method(const std::string&                  name,
                        const std::type_index&              return_type,
                        const mge::script::signature&       sig,
                        const mge::script::invoke_function& invoke);

        void add_static_method(const std::string&                  name,
                               const std::type_index&              return_type,
                               const mge::script::signature&       sig,
                               const mge::script::invoke_function& invoke);

        void
        set_destructor(const mge::script::invoke_function& delete_ptr,
                       const mge::script::invoke_function& delete_shared_ptr);

        const constructor* select_constructor(int nargs, lua_State* L) const;

    private:
        void create_instance_metatable();
        void load_instance_metatable();
        void create_type_metatable();
        void define_construction();
        void define_index();

        mge::script::type_details_ref                            m_details;
        std::map<size_t, std::vector<constructor>>               m_constructors;
        std::map<const char*, field, cstring_less>               m_fields;
        std::map<const char*, std::vector<method>, cstring_less> m_methods;
        std::map<const char*, std::vector<method>, cstring_less>
                                            m_static_methods;
        const mge::script::invoke_function* m_delete_ptr;
        const mge::script::invoke_function* m_delete_shared_ptr;
    };

} // namespace mge::lua