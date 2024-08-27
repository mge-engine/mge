// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/call_context.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

#include <iostream>
#include <string>

namespace mge::script {
    class MGESCRIPT_EXPORT function_data
    {
    public:
        function_data(const char* name, void* fptr);
        ~function_data() = default;

        const std::string& name() const noexcept { return m_name; }

        void set_invoker(const invoke_function& invoker)
        {
            m_invoker = invoker;
        }

        const invoke_function& invoker() const noexcept { return m_invoker; }

        void invoke(call_context& context) const;

    private:
        std::string          m_name;
        void*                m_fptr;
        module_data_weak_ref m_module;
        invoke_function      m_invoker;
    };
} // namespace mge::script