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
        ~python_module();
        std::string full_name() const;

    private:
        PyModuleDef         m_py_module_def;
        PyObject*           m_py_module;
        mge::script::module m_module;
        std::string         m_name;
    };
} // namespace mge::python