// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/reflection/module_details.hpp"

#include "pyobject_ref.hpp"
#include "python_fwd.hpp"

namespace mge::python {
    class python_context;

    class python_module
    {
    public:
        python_module(python_context&                       context,
                      const reflection::module_details_ref& details);
        ~python_module();

        const std::string& full_name() const noexcept
        {
            return m_full_name;
        }

        const std::string& python_name() const noexcept
        {
            return m_python_name;
        }

        const pyobject_ref& py_module() const noexcept
        {
            return m_py_module;
        }

        void add(const python_type_ref& type);

    private:
        void create_module();

        python_context&                m_context;
        reflection::module_details_ref m_details;
        std::string                    m_full_name;
        std::string                    m_python_name;
        pyobject_ref                   m_py_module;
        std::vector<python_type_ref>   m_types;
    };

} // namespace mge::python
