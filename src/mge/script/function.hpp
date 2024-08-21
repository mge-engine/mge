// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/function_data.hpp"
#include "mge/script/script_fwd.hpp"

namespace mge::script {

    template <typename R, typename... Args> class function
    {
    public:
        function() {}

        function(const function& f)
            : m_data(f.m_data)
        {}
        function(function&& f) noexcept
            : m_data(std::move(f.m_data))
        {}
        function& operator=(const function& f)
        {
            m_data = f.m_data;
            return *this;
        }
        function& operator=(function&& f) noexcept
        {
            m_data = std::move(f.m_data);
            return *this;
        }

        function(const char* name, R (*f)(Args...))
            : m_data(std::make_shared<function_data>(
                  name, reinterpret_cast<void*>(f)))
        {}

        ~function() = default;

        const function_data_ref& data() const noexcept { return m_data; }

    private:
        function_data_ref m_data;
    };

} // namespace mge::script