// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "python_module.hpp"
#include "python_error.hpp"

namespace mge ::python {
    python_module::python_module(const mge::script::module& m)
        : m_py_module(nullptr)
        , m_module(m)
        , m_name(m_module.name())
    {
        if (!m_name.empty()) {
            m_py_module = PyImport_AddModule(m_name.c_str());
        } else {
            m_name = "__main__";
            m_py_module = PyImport_AddModule(m_name.c_str());
        }
    }

    python_module::~python_module() { Py_XDECREF(m_py_module); }

    std::string python_module::full_name() const
    {
        return m_module.full_name();
    }

    void python_module::add_module(const python_module_ref& m)
    {
        add_object(m->m_name.c_str(), m->m_py_module);
    }

    void python_module::add_object(const char* name, PyObject* object)
    {
        if (PyModule_AddObjectRef(m_py_module, name, object)) {
            error::check_error();
        }
    }
} // namespace mge::python