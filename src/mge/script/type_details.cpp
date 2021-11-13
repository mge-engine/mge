// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/type_details.hpp"

namespace mge::script {
    type_details::type_details(const std::string_view name)
        : m_name(name)
    {}

    std::string_view type_details::name() const { return m_name; }
} // namespace mge::script