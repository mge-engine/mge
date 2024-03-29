// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/call_context.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/signature.hpp"
#include "mge/script/traits.hpp"

#include <functional>
#include <string>
#include <tuple>
#include <typeindex>
#include <vector>

namespace mge::script {

    /**
     * @brief Details of a type.
     */
    class MGESCRIPT_EXPORT type_details
        : public std::enable_shared_from_this<type_details>
    {
    public:
        /**
         * @brief Construct a new type details object
         *
         * @param name          type name
         * @param ti            C++ type info
         * @param t             type traits
         * @param size          type binary size
         * @param used_name     used name (like 'string' for
         * 'std::basic_string<...>')
         */
        type_details(const std::string_view& name,
                     const std::type_index&  ti,
                     const traits&           t,
                     size_t                  size,
                     const char*             used_name);

        type_details(const type_details&) = delete;
        type_details& operator=(const type_details&) = delete;

        virtual ~type_details() = default;

        /**
         * @brief Name of type (used name).
         * @return type name (used name)
         */
        const std::string& name() const;

        /**
         * @brief Unique name of type.
         * @return type name (unique name)
         */
        const std::string& unique_name() const;

        /**
         * @brief Reference to module, if any.
         *
         * @return type's module
         */
        const module_details_weak_ref& module() const;

        /**
         * @brief C++ <tt>std::type_index</tt> of type.
         *
         * @return type index of type
         */
        const std::type_index& type_index() const;

        /**
         * @brief Type traits.
         *
         * @return type traits
         */
        const mge::script::traits& traits() const;

        /**
         * @brief Enclosing type for nested types.
         *
         * @return enclosing type
         */
        const type_details_ref& enclosing_type() const;

        /**
         * @brief Whether this type is a sub type of another.
         *
         * @return true if this type is enclosed in another type
         */
        bool is_subtype() const;

        /**
         * @brief Binary size.
         *
         * @return Binary size of type.
         */
        size_t size() const;

        /**
         * @brief Set the type's module.
         *
         * @param m type's module
         */
        void set_module(const module_details_ref& m);

        /**
         * @brief Set the enclosing type.
         *
         * @param t enclosing type
         */
        void set_enclosing_type(const type_details_ref& t);

        /**
         * @brief Get enclosing module.
         * This is either the module of the type, or the enclosing module
         * of the enclosing type in case of nested types.
         *
         * @return enclosing module
         */
        module_details_ref enclosing_module() const;

        /**
         * @brief Lookup details by type index.
         *
         * @param ti type index
         * @return found type details, if not found exception is raised
         */
        static type_details_ref get_first_match(const std::type_index& ti);
        /**
         * @brief Create type details.
         *
         * @param ti C++ type index
         * @param name type name
         * @param tr traits
         * @param size type binary size
         *
         * @return created type
         */
        static type_details_ref create(const std::type_index&     ti,
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
        std::string             m_unique_name;
        module_details_weak_ref m_module;
        std::type_index         m_type_index;
        mge::script::traits     m_traits;
        size_t                  m_size;
        type_details_ref        m_enclosing_type;
    };

    /**
     * @brief Type details of an enum type.
     */
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

    /**
     * @brief Type details of a class type.
     */
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
                               const invoke_function& new_shared_)
                : signature(s)
                , new_at(new_at_)
                , new_shared(new_shared_)
            {}

            mge::script::signature signature;
            invoke_function        new_at;
            invoke_function        new_shared;
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
                             const invoke_function& new_shared);
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

        const auto& constructors() const { return m_constructors; }
        const auto& fields() const { return m_fields; }
        const auto& transfer_shared() const { return m_transfer_shared; }

    private:
        using type_map =
            std::unordered_map<std::type_index, std::vector<type_details_ref>>;

        std::vector<type_details_ref> m_bases;
        destructor                    m_destructor;
        std::vector<constructor>      m_constructors;
        mge::script::transfer_shared  m_transfer_shared;
        std::vector<field>            m_fields;
        std::vector<method>           m_methods;
        type_map                      m_types;
    };

} // namespace mge::script