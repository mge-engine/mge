// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/dllexport.hpp"

#include <typeindex>
#include <typeinfo>

namespace mge::reflection {

    /**
     * Represents a unique identifier for a type, including its const and
     * volatile qualifiers.
     */
    class MGEREFLECTION_EXPORT type_identifier
    {
    public:
        type_identifier(const std::type_info& type_info,
                        bool                  is_const,
                        bool                  is_volatile) noexcept;
        ~type_identifier() = default;

        type_identifier(const type_identifier&) = default;
        type_identifier(type_identifier&&) noexcept = default;
        type_identifier& operator=(const type_identifier&) = default;
        type_identifier& operator=(type_identifier&&) noexcept = default;

        const std::type_index& type_index() const noexcept
        {
            return m_type_index;
        }

        bool is_const() const noexcept
        {
            return m_is_const;
        }

        bool is_volatile() const noexcept
        {
            return m_is_volatile;
        }

    private:
        std::type_index m_type_index;
        bool            m_is_const;
        bool            m_is_volatile;
    };
} // namespace mge::reflection