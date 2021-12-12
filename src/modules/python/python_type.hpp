// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/memory.hpp"
#include "python.hpp"
#include <string>
namespace mge::python {

    MGE_DECLARE_REF(python_type);

    class python_type
    {
    public:
        python_type(const std::string& name, size_t size);
        ~python_type() = default;

        PyObject* materialize_type();

    private:
        std::string m_name;
        PyType_Spec m_spec;
    };

} // namespace mge::python