// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/function_details.hpp"
#include "mge/script/module_details.hpp"
#include "mge/script/type_details.hpp"
#include "mge/script/visitor.hpp"

namespace mge::script {
    function_details::function_details(const std::string& name)
        : m_name(name)
    {}

    const std::string& function_details::name() const { return m_name; }

    void function_details::apply(visitor& v) { v.visit(*this); }

} // namespace mge::script