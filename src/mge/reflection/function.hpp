// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/function_details.hpp"
#include "mge/reflection/reflection_fwd.hpp"

namespace mge::reflection {

    template <typename Result, typename... Args> class function
    {
    public:
        using function_type = Result(Args...);

        function() = default;
        ~function() = default;
        function(function&&) = default;
        function& operator=(function&&) = default;
        function(const function&) = default;
        function& operator=(const function&) = default;

        function(const char* name, Result (*func)(Args...))
        {
            m_details = std::make_shared<function_details>(name, func);
        }

        const function_details_ref& details() const noexcept
        {
            return m_details;
        }

    private:
        function_details_ref m_details;
    };

} // namespace mge::reflection
