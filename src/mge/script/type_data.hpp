// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/memory.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

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
        enum class type_kind
        {
            ENUM,
            CLASS,
            POD
        };

        type_data(const std::type_info& ti, type_kind kind);
        type_data(const type_data&) = delete;
        type_data& operator=(const type_data&) = delete;

        type_data(type_data&& t)
            : m_type_info(t.m_type_info)
            , m_module(std::move(t.m_module))
            , m_name(std::move(t.m_name))
            , m_initialized(t.m_initialized)
        {}

        type_data& operator=(type_data&& other)
        {
            m_type_info = std::move(other.m_type_info);
            m_module = std::move(other.m_module);
            m_name = std::move(other.m_name);
            m_initialized = std::move(other.m_initialized);
            return *this;
        }

        ~type_data();

        static type_data_ref get(const std::type_info& ti);
        static type_data_ref create(const std::type_info& ti, type_kind kind);

        const std::string& name() const;
        struct enum_details
        {
            type_data_ref                                underlying_type;
            std::vector<std::pair<int64_t, std::string>> values;
        };

        struct class_details
        {};

        struct pod_details
        {};

        type_data::enum_details&  enum_specific();
        type_data::class_details& class_specific();

        const type_data::enum_details&  enum_specific() const;
        const type_data::class_details& class_specific() const;

    private:
        friend class module_data;

        using details_type = std::variant<std::monostate,
                                          type_data::enum_details,
                                          type_data::class_details,
                                          type_data::pod_details>;

        const std::type_info* m_type_info{nullptr};
        module_data_weak_ref  m_module;
        mutable std::string   m_name;
        details_type          m_details;
        bool                  m_initialized{false};
    };

} // namespace mge::script