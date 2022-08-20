// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/call_context.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/signature.hpp"
#include "mge/script/traits.hpp"

#include <string>
#include <tuple>
#include <typeindex>
#include <vector>

namespace mge::script {

    class MGESCRIPT_EXPORT type_details
        : public std::enable_shared_from_this<type_details>
    {
    public:
        type_details(const std::string&     name,
                     const std::type_index& ti,
                     const traits&          t,
                     size_t                 size,
                     const char*            used_name);

        virtual ~type_details() = default;

        const std::string&             name() const;
        const std::string&             automatic_name() const;
        const module_details_weak_ref& module() const;
        const std::type_index&         type_index() const;
        const mge::script::traits&     traits() const;
        const type_details_ref&        enclosing_type() const;
        size_t                         size() const;
        size_t                         shared_ptr_size() const;

        void set_module(const module_details_ref& m);
        void set_enclosing_type(const type_details_ref& t);

        /**
         * @brief Lookup details by type index.
         *
         * @param ti type index
         * @return found type details, if not found exception is raised
         */
        static type_details_ref get(const std::type_index& ti);
        static type_details_ref get_or_create(const std::type_index&     ti,
                                              const std::string&         name,
                                              const mge::script::traits& tr,
                                              size_t                     size);

        /**
         * @brief Applies a visitor.
         *
         * @oaram self shared ppinter to this object
         * @param v visitor
         */
        virtual void apply(const type_details_ref& self, visitor& v);

    private:
        std::string             m_automatic_name;
        std::string             m_name;
        module_details_weak_ref m_module;
        std::type_index         m_type_index;
        mge::script::traits     m_traits;
        size_t                  m_size;
        type_details_ref        m_enclosing_type;
    };

    class MGESCRIPT_EXPORT enum_type_details : public type_details
    {
    public:
        enum_type_details(const std::string&         name,
                          const std::type_index&     ti,
                          const mge::script::traits& t,
                          size_t                     size,
                          const char*                used_name = nullptr);

        virtual ~enum_type_details() = default;

        void apply(const type_details_ref& self, visitor& v) override;

        void enum_value(const std::string& name, int64_t value);

    private:
        using value = std::tuple<std::string, int64_t>;
        std::vector<value> m_values;
    };

    class MGESCRIPT_EXPORT class_type_details : public type_details
    {
    public:
        struct constructor
        {
            inline constructor() = default;
            inline constructor(const constructor&) = default;
            inline constructor(constructor&&) = default;

            constructor& operator=(const constructor&) = default;
            constructor& operator=(constructor&&) = default;

            inline constructor(const signature&       s,
                               const invoke_function& new_at_,
                               const invoke_function& make_shared_)
                : signature(s)
                , new_at(new_at_)
                , make_shared(make_shared_)
            {}

            mge::script::signature signature;
            invoke_function        new_at;
            invoke_function        make_shared;
        };

        struct destructor
        {
            inline destructor() = default;
            inline destructor(const destructor&) = default;
            inline destructor(destructor&&) = default;
            destructor& operator=(const destructor&) = default;
            destructor& operator=(destructor&&) = default;
            inline destructor(const invoke_function& delete_ptr_,
                              const invoke_function& delete_shared_ptr_)
                : delete_ptr(delete_ptr_)
                , delete_shared_ptr(delete_shared_ptr_)
            {}

            bool empty() const { return !delete_ptr && !delete_shared_ptr; }

            invoke_function delete_ptr;
            invoke_function delete_shared_ptr;
        };

        struct field
        {
            inline field() = default;
            inline field(const field&) = default;
            inline field(field&&) = default;

            field& operator=(const field&) = default;
            field& operator=(field&&) = default;

            inline field(const std::string&      n,
                         const type_details_ref& t,
                         const invoke_function&  g)
                : name(n)
                , type(t)
                , getter(g)
            {}

            inline field(const std::string&      n,
                         const type_details_ref& t,
                         const invoke_function&  g,
                         const invoke_function&  s)
                : name(n)
                , type(t)
                , getter(g)
                , setter(s)
            {}

            std::string      name;
            type_details_ref type;
            invoke_function  getter;
            invoke_function  setter;
        };

        struct method
        {
            inline method() = default;
            inline method(const method&) = default;
            inline method(method&&) = default;

            method& operator=(const method&) = default;
            method& operator=(method&&) = default;

            inline method(const std::string&            n,
                          const std::type_index&        r,
                          const mge::script::signature& s,
                          const invoke_function&        m)
                : name(n)
                , result_type(r)
                , signature(s)
                , invoke(m)
                , is_static(false)
            {}

            inline method(const std::string&            n,
                          const std::type_index&        r,
                          const mge::script::signature& s,
                          const invoke_function&        m,
                          bool                          is_static_)
                : name(n)
                , result_type(r)
                , signature(s)
                , invoke(m)
                , is_static(is_static_)
            {}

            std::string            name;
            std::type_index        result_type;
            mge::script::signature signature;
            invoke_function        invoke;
            bool                   is_static;
        };

        class_type_details(const std::string&         name,
                           const std::type_index&     ti,
                           const mge::script::traits& t,
                           size_t                     size,
                           const char*                used_name = nullptr);

        virtual ~class_type_details() = default;
        void apply(const type_details_ref& self, visitor& v) override;

        void set_base(const type_details_ref& base_details);
        void set_destructor(const invoke_function& delete_ptr,
                            const invoke_function& delete_shared_ptr);
        void add_constructor(const signature&       s,
                             const invoke_function& new_at,
                             const invoke_function& make_shared);
        void add_field(const std::string&      name,
                       const type_details_ref& t,
                       const invoke_function&  getter);
        void add_field(const std::string&      name,
                       const type_details_ref& t,
                       const invoke_function&  getter,
                       const invoke_function&  setter);

        void add_method(const std::string&     name,
                        const std::type_index& return_type,
                        const signature&       sgn,
                        const invoke_function& invoke);
        void add_static_method(const std::string&     name,
                               const std::type_index& return_type,
                               const signature&       sgn,
                               const invoke_function& invoke);

        void add_type(const type_details_ref& t);

    private:
        using type_map = std::unordered_map<std::type_index, type_details_ref>;

        std::vector<type_details_ref> m_bases;
        destructor                    m_destructor;
        std::vector<constructor>      m_constructors;
        std::vector<field>            m_fields;
        std::vector<method>           m_methods;
        type_map                      m_types;
    };

} // namespace mge::script