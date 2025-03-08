// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "python_context.hpp"
#include "python_module.hpp"
#include <memory>

namespace mge::python {

    class component
    {
    public:
        static void      create_registry_type(const python_module_ref& module,
                                              python_context*          context);
        static PyObject* register_component(PyObject* self, PyObject* args);
        static python_type_ref component_base_type(PyObject* component_class);
    };

} // namespace mge::python
