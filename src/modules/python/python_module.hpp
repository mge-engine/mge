// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/module.hpp"
#include "python.hpp"

namespace mge::python {
    class python_module
    {
    public:
        python_module(const mge::script::module& m);
        ~python_module();

        PyObject* py_module() const { return m_py_module; }

    private:
        PyObject*           m_py_module;
        mge::script::module m_module;
    };
} // namespace mge::python