// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/type_identifier.hpp"

namespace mge::reflection {

    type_identifier::type_identifier(const std::type_info& type_info,
                                     bool                  is_const,
                                     bool                  is_volatile,
                                     bool is_reference) noexcept
        : m_type_index(type_info)
        , m_is_const(is_const)
        , m_is_volatile(is_volatile)
        , m_is_reference(is_reference)
    {}

} // namespace mge::reflection