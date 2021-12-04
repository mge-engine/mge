// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "python_module.hpp"

namespace mge ::python {
    python_module::python_module(const mge::script::module& m)
        : m_py_module(nullptr)
        , m_module(m)
        , m_name(m_module.name())
    {
        new (&m_py_module_def) PyModuleDef{
            PyModuleDef_HEAD_INIT,
            m_name.c_str(), /* m_name */
            nullptr,        /* m_doc */
            -1,             /* m_size */
            nullptr,        /* m_methods */
            nullptr,        /* m_slots */
            nullptr,        /* m_traverse */
            nullptr,        /* m_clear */
            nullptr,        /* m_free */
        };

        m_py_module = PyModule_Create(&m_py_module_def);
    }

    python_module::~python_module() { Py_XDECREF(m_py_module); }
} // namespace mge::python