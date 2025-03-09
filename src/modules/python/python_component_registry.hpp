// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "python_context.hpp"
#include "python_module.hpp"
#include <memory>

namespace mge::python {

    class python_component_registry
    {
    public:
        static void create(const python_module_ref& module,
                           python_context*          context);

        static PyObject* register_component(PyObject* self, PyObject* args);

        static PyObject*
                   tp_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
        static int tp_init(PyObject* self, PyObject* args, PyObject* kwds);

    private:
        PyObject* register_component(const char* name,
                                     PyObject*   component_class);

        // clang-format off
        PyObject_HEAD
        python_context* context;
        // clang-format on
    };

} // namespace mge::python
