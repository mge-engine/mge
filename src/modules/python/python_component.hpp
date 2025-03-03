// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "python_module.hpp"

namespace mge::python {

    class component
    {
    public:
        static void register_component_type(const python_module_ref& module);
    };

} // namespace mge::python
