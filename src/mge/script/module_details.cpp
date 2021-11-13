// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/module_details.hpp"
namespace mge::script {
    module_details::module_details(const module_details_ref& parent,
                                   const std::string_view    name)
        : m_parent(parent)
        , m_name(name)
    {}

    module_details::~module_details() {}

} // namespace mge::script