// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/function_data.hpp"
#include "mge/core/trace.hpp"
#include <iostream>
namespace mge {
    MGE_USE_TRACE(SCRIPT);
}
namespace mge::script {

    function_data::function_data(const char* name, void* fptr)
        : m_name(name)
        , m_fptr(fptr)
    {}

    void function_data::invoke(call_context& context) const
    {
        m_invoker(context);
    }

    void function_data::add_dependency(const dependency& dep)
    {
        m_dependencies.insert(dep);
    }

} // namespace mge::script
