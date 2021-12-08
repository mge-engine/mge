// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/function_details.hpp"
#include "mge/script/module.hpp"
#include "mge/script/module_details.hpp"
#include "mge/script/type_details.hpp"
#include "mge/script/visitor.hpp"

namespace mge::script {
    function_details::function_details(
        const std::string&                           name,
        const function_details::invocation_function& invoke)
        : m_name(name)
        , m_invoke(invoke)
    {}

    const std::string& function_details::name() const { return m_name; }

    mge::script::module function_details::module() const
    {
        if (m_module.expired()) {
            MGE_THROW(illegal_state)
                << "Function '" << name() << "' is incomplete, no module set";
        }
        auto m = m_module.lock();
        return mge::script::module(m);
    }

    void function_details::apply(visitor& v) { v.visit(*this); }

    const function_details::invocation_function&
    function_details::invoke() const
    {
        return m_invoke;
    }

} // namespace mge::script