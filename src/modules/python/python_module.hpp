// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/memory.hpp"
#include "mge/script/module.hpp"
#include "python.hpp"

namespace mge::python {

    MGE_DECLARE_REF(python_module);

    class python_module
    {
    public:
        python_module(const mge::script::module& m);
        python_module(const python_module_ref& parent, const mge::script::module& m);
        ~python_module();

        PyObject* py_module() const noexcept { return m_py_module; }
        const mge::script::module& module() const noexcept { return m_module; }

    private:
        PyObject*           m_py_module;
        mge::script::module m_module;
    };
} // namespace mge::python