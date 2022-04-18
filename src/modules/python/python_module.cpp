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
            m_py_module_dict = context.main_dict();
            Py_INCREF(m_py_module_dict);
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

    python_module::~python_module()
    {
        Py_CLEAR(m_py_module_dict);
        Py_CLEAR(m_py_module);
    }

} // namespace mge::python