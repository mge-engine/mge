// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/type_details.hpp"
#include "python.hpp"

#include <memory>

namespace mge::python {

    /**
     * @brief Header embedded in every Python class instance object.
     *
     * Layout in memory: ob_base (PyObject) followed by type pointer and
     * a shared_ptr<void> owning the C++ object.
     */
    struct python_instance_header
    {
        PyObject                             ob_base;
        const mge::reflection::type_details* type;
        std::shared_ptr<void>                object;
    };

} // namespace mge::python
