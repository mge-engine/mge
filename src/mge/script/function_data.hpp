// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/call_context.hpp"
#include "mge/script/dependency.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/type_data.hpp"

#include <iostream>
#include <set>
#include <string>

namespace mge::script {
    class MGESCRIPT_EXPORT function_data
    {
    public:
        function_data(const char* name, void* fptr);
        ~function_data() = default;

        void set_module(const module_data_ref& m)
        {
            m_module = m;
            m_dependencies.insert(dependency(m));
        }

        const module_data_weak_ref& module() const noexcept { return m_module; }

        const std::string& name() const noexcept { return m_name; }

        void set_return_type(const type_identifier& t) { m_return_type = t; }

        const type_identifier& return_type() const noexcept
        {
            return m_return_type;
        }

        void set_signature(const type_data::call_signature& s)
        {
            m_signature = s;
        }

        const type_data::call_signature& signature() const noexcept
        {
            return m_signature;
        }

        void set_invoker(const invoke_function& invoker)
        {
            m_invoker = invoker;
        }

        void add_dependency(const dependency& dep);

        const dependency_set& dependencies() const noexcept
        {
            return m_dependencies;
        }

        const invoke_function& invoker() const noexcept { return m_invoker; }

        void invoke(call_context& context) const;

    private:
        std::string               m_name;
        void*                     m_fptr;
        module_data_weak_ref      m_module;
        type_identifier           m_return_type;
        type_data::call_signature m_signature;
        invoke_function           m_invoker;
        dependency_set            m_dependencies;
    };
} // namespace mge::script