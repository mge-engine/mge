// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "python_module.hpp"
#include "mge/core/trace.hpp"
#include "python_context.hpp"
#include "python_error.hpp"

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {

    python_module::python_module(python_context&            context,
                                 const mge::script::module& m)
        : m_context(context)
        , m_py_module(nullptr)
        , m_py_module_dict(nullptr)
        , m_module(m)
    {
        if (m_module.is_root()) {
            MGE_DEBUG_TRACE(PYTHON) << "Creating __main__ module";
            m_py_module = PyImport_AddModule("__main__");
            m_py_module_dict = PyModule_GetDict(m_py_module);
        } else {
            MGE_DEBUG_TRACE(PYTHON) << "Creating " << m.name() << " module";
            m_py_module = PyImport_AddModule(m.name().c_str());
            m_py_module_dict = PyModule_GetDict(m_py_module);
        }
        error::check_error();
    }

    python_module::python_module(python_context&            context,
                                 const python_module_ref&   parent,
                                 const mge::script::module& m)
        : m_context(context)
        , m_py_module(nullptr)
        , m_py_module_dict(nullptr)
        , m_module(m)
    {
        MGE_DEBUG_TRACE(PYTHON) << "Creating '" << m.name() << "' module";
        m_py_module = PyImport_AddModule(m.name().c_str());
        m_py_module_dict = PyModule_GetDict(m_py_module);
        PyDict_SetItemString(parent->py_module_dict(),
                             m.name().c_str(),
                             m_py_module);
        error::check_error();
    }

    void python_module::interpreter_lost()
    {
        m_py_module_dict = nullptr;
        m_py_module = nullptr;
        for (const auto& t : m_types) {
            t->interpreter_lost();
        }
    }

    python_module::~python_module()
    {
        Py_CLEAR(m_py_module_dict);
        Py_CLEAR(m_py_module);
    }

    void python_module::add_type(const python_type_ref& type)
    {
        MGE_DEBUG_TRACE(PYTHON) << "Adding type '" << type->local_name()
                                << "' to module '" << m_module.name() << "'";

        PyObject* py_type = type->py_type();
        if (py_type) {
            if (!type->is_subtype()) {
                PyModule_AddObjectRef(m_py_module,
                                      type->local_name().c_str(),
                                      py_type);
                error::check_error();
            }
            m_types.push_back(type);
        }
    }

} // namespace mge::python
