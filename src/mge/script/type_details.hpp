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
        size_t                         size() const;

        void set_module(const module_details_ref& m);

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

            inline constructor(const signature& s, const invoke_function& ctor)
                : signature(s)
                , function(ctor)
            {}

            mge::script::signature signature;
            invoke_function        function;
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
            {}

            std::string            name;
            std::type_index        result_type;
            mge::script::signature signature;
            invoke_function        invoke;
        };

        class_type_details(const std::string&         name,
                           const std::type_index&     ti,
                           const mge::script::traits& t,
                           size_t                     size,
                           const char*                used_name = nullptr);

        virtual ~class_type_details() = default;
        void apply(const type_details_ref& self, visitor& v) override;

        void set_base(const type_details_ref& base_details);
        void set_destructor(const invoke_function& dtor);
        void add_constructor(const signature& s, const invoke_function& ctor);
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

    private:
        std::vector<type_details_ref> m_bases;
        invoke_function               m_destructor;
        std::vector<constructor>      m_constructors;
        std::vector<field>            m_fields;
        std::vector<method>           m_methods;
    };

} // namespace mge::script