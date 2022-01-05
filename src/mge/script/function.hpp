// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include <type_traits>

namespace mge::script {

    class MGE_SCRIPT_EXPORT function_base
    {
    private:
        function_details_ref m_details;
    };

    template <typename R, typename... Args> class c_function
    {
    public:
        c_function(const std::string& name, R (*fptr)(Args... args)) {}
    };

} // namespace mge::script