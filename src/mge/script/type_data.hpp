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

namespace mge::script {

    /**
     * Data on a type.
     */
    class MGESCRIPT_EXPORT type_data
    {
    public:
        type_data(const std::type_info& ti);
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
        static type_data_ref create(const std::type_info& ti);

        const std::string& name() const;

    private:
        friend class module_data;

        const std::type_info* m_type_info{nullptr};
        module_data_weak_ref  m_module;
        mutable std::string   m_name;
        bool                  m_initialized{false};
    };

} // namespace mge::script