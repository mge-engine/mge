// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/reflection/module_details.hpp"

#include "pyobject_ref.hpp"

namespace mge::python {
    class python_context;

    class python_module
    {
    public:
        python_module(python_context&                       context,
                      const reflection::module_details_ref& details);
        ~python_module();

    private:
        python_context&                m_context;
        reflection::module_details_ref m_details;
        pyobject_ref                   m_py_module;
    };

} // namespace mge::python
