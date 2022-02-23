// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/variable_details.hpp"
#include "mge/script/visitor.hpp"

namespace mge::script {

    variable_details::variable_details(const std::string&      name,
                                       void*                   address,
                                       const type_details_ref& type)
        : m_name(name)
        , m_address(address)
        , m_type(type)
    {}

    const std::string& variable_details::name() const { return m_name; }

    void* variable_details::address() const { return m_address; }

    const type_details_ref& variable_details::type() const { return m_type; }

    void variable_details::apply(visitor& v) { v.visit(*this); }

} // namespace mge::script