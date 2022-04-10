// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/memory.hpp"
#include "mge/script/module.hpp"
#include "python.hpp"

namespace mge::python {

    class python_context;

    MGE_DECLARE_REF(python_module);

    class python_module
    {
    public:
        python_module(python_context& context, const mge::script::module& m);
        python_module(python_context&            context,
                      const python_module_ref&   parent,
                      const mge::script::module& m);
        ~python_module();

        PyObject* py_module() const noexcept { return m_py_module; }
        PyObject* py_module_dict() const noexcept { return m_py_module_dict; }
        const mge::script::module& module() const noexcept { return m_module; }

    private:
        python_context&     m_context;
        PyObject*           m_py_module;
        PyObject*           m_py_module_dict;
        mge::script::module m_module;
    };
} // namespace mge::python