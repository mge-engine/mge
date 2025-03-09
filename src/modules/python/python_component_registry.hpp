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
        // clang-format off
        PyObject_HEAD
        python_context* context;
        // clang-format on
    };

} // namespace mge::python
