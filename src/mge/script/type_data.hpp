// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/memory.hpp"
#include "mge/script/call_context.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

#include <iostream>
#include <optional>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <variant>
#include <vector>

namespace mge::script {

    /**
     * Data on a type.
     */
    class MGESCRIPT_EXPORT type_data
    {
    public:
        using call_signature = std::vector<std::type_index>;

        enum class type_kind
        {
            ENUM,
            CLASS,
            POD,
            POINTER,
            REFERENCE,
            RVALUE_REFERENCE,
            VOID
        };

        type_data(const std::type_info& ti, type_kind kind);
        type_data(const type_data&) = delete;
        type_data& operator=(const type_data&) = delete;

        type_data(type_data&& t)
            : m_type_info(t.m_type_info)
            , m_module(std::move(t.m_module))
            , m_details(std::move(t.m_details))
        {}

        type_data& operator=(type_data&& other)
        {
            m_type_info = std::move(other.m_type_info);
            m_module = std::move(other.m_module);
            m_details = std::move(other.m_details);
            return *this;
        }

        ~type_data();

        static type_data_ref get(const std::type_info& ti);
        static type_data_ref create(const std::type_info& ti, type_kind kind);

        void set_module(const module_data_ref& m) { m_module = m; }

        bool is_pod() const;
        bool is_enum() const;
        bool is_class() const;
        bool is_pointer() const;
        bool is_reference() const;
        bool is_rvalue_reference() const;
        bool is_void() const;

        bool is_string() const;
        bool is_wstring() const;
        bool is_const() const;
        bool is_volatile() const;

        bool exposed_directly() const;

        std::string name() const;
        struct enum_details
        {
            type_data_ref                                underlying_type;
            std::vector<std::pair<int64_t, std::string>> values;
        };

        struct class_details
        {
            std::vector<type_data_ref>   base_classes;
            bool                         is_string{false};
            bool                         is_wstring{false};
            bool                         is_abstract{false};
            size_t                       size{0};
            mge::script::invoke_function destroy;
            mge::script::invoke_function default_construct;
            std::vector<std::pair<call_signature, mge::script::invoke_function>>
                constructors;

            // name, type, getter, setter
            std::vector<std::tuple<std::string,
                                   type_data_ref,
                                   mge::script::invoke_function,
                                   mge::script::invoke_function>>
                fields;
            std::vector<std::tuple<std::string,
                                   type_data_ref,
                                   call_signature,
                                   mge::script::invoke_function>>
                methods;

            std::vector<std::tuple<std::string,
                                   type_data_ref,
                                   call_signature,
                                   mge::script::invoke_function>>
                functions;

            type_data_ref      proxy_type;
            type_data_weak_ref interface_type;
        };

        struct pod_details
        {
            size_t size{0};
        };

        struct pointer_details
        {
            type_data_ref pointee;
            bool          is_const{false};
            bool          is_volatile{false};
        };

        struct reference_details
        {
            type_data_ref referencee;
            bool          is_const{false};
            bool          is_volatile{false};
        };

        struct rvalue_reference_details
        {
            type_data_ref referencee;
            bool          is_const{false};
            bool          is_volatile{false};
        };

        struct void_details
        {};

        type_data::enum_details&             enum_specific();
        type_data::class_details&            class_specific();
        type_data::pod_details&              pod_specific();
        type_data::pointer_details&          pointer_specific();
        type_data::reference_details&        reference_specific();
        type_data::rvalue_reference_details& rvalue_reference_specific();
        type_data::void_details&             void_specific();

        const type_data::enum_details&      enum_specific() const;
        const type_data::class_details&     class_specific() const;
        const type_data::pod_details&       pod_specific() const;
        const type_data::pointer_details&   pointer_specific() const;
        const type_data::reference_details& reference_specific() const;
        const type_data::rvalue_reference_details&
                                       rvalue_reference_specific() const;
        const type_data::void_details& void_specific() const;

    private:
        friend class module_data;

        using details_type = std::variant<std::monostate,
                                          type_data::enum_details,
                                          type_data::class_details,
                                          type_data::pod_details,
                                          type_data::pointer_details,
                                          type_data::reference_details,
                                          type_data::rvalue_reference_details,
                                          type_data::void_details>;

        const std::type_info* m_type_info{nullptr};
        module_data_weak_ref  m_module;
        details_type          m_details;
    };

} // namespace mge::script