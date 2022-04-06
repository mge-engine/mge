// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "python_module.hpp"
#include "mge/core/trace.hpp"
#include "python_error.hpp"

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {
    python_module::python_module(const mge::script::module& m)
        : m_py_module(nullptr)
        , m_module(m)
    {
        if (m_module.is_root()) {
            MGE_DEBUG_TRACE(PYTHON) << "Creating __main__ module";
            m_py_module = PyImport_AddModule("__main__");
        } else {
            MGE_DEBUG_TRACE(PYTHON) << "Creating " << m.name() << " module";
            m_py_module = PyImport_AddModule(m.name().c_str());
        }
        error::check_error();
    }

    python_module::~python_module() { Py_XDECREF(m_py_module); }

} // namespace mge::python