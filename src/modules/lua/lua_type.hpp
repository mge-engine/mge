// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/cstring.hpp"
#include "mge/core/memory.hpp"
#include "mge/core/small_vector.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/type_details.hpp"

#include "lua.hpp"
#include "lua_context.hpp"

#include <map>

namespace mge::lua {

    MGE_DECLARE_REF(type);

    class type
    {
    public:
        type(lua_context&                         context,
             const mge::script::type_details_ref& details);
        ~type();

        const mge::script::type_details& details() const { return *m_details; }
        const std::string& local_name() const { return m_details->name(); }

        void add_enum_value(const std::string& name, int64_t enum_value);
        void add_constructor(const mge::script::signature&       s,
                             const mge::script::invoke_function& new_at,
                             const mge::script::invoke_function& new_shared);

        void
        set_destructor(const mge::script::invoke_function& delete_ptr,
                       const mge::script::invoke_function& delete_shared_ptr);

        void add_to_parent(lua_context& context);
        void push_type_table();
        void pop_type_table();
        void add_type(const lua::type_ref& t);

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

        bool            materialized() const { return m_materialized; }
        std::type_index type_index() const { return m_details->type_index(); }

    private:
        void define_construction();
        void load_metatable(lua_State* L);
        void add_index_method();

        lua_context&                              m_context;
        const mge::script::type_details_ref&      m_details;
        std::optional<std::vector<lua::type_ref>> m_types;
        bool                                      m_materialized;
        bool                                      m_ctor_defined;

        struct constructor
        {
            const mge::script::signature*       s;
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

        const constructor* select_constructor(int nargs, lua_State* L) const;

        struct method
        {
            method(const std::string* name_)
                : name(name_)
            {}

            const std::string* name;
            struct details
            {
                const std::type_index*              return_type;
                const mge::script::signature*       signature;
                const mge::script::invoke_function* invoke;
            };
            mge::small_vector<details, 1> overloads;
        };

        static int construct(lua_State* L);
        static int destruct(lua_State* L);
        static int call(lua_State *L);
        static int index(lua_State* L);

        std::map<const char*, field, cstring_less> m_fields;

        std::map<const char*, method*, cstring_less> m_methods;
        std::vector<std::unique_ptr<method>>         m_all_methods;

        std::map<size_t, std::vector<constructor>> m_constructors;
        const mge::script::invoke_function*        m_delete_ptr;
        const mge::script::invoke_function*        m_delete_shared_ptr;
    };

} // namespace mge::lua