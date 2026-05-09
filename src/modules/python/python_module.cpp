// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "python_module.hpp"
#include "python_error.hpp"

#include "gil_lock.hpp"

namespace mge::python {
    python_module::python_module(python_context&                       context,
                                 const reflection::module_details_ref& details)
        : m_context(context)
        , m_details(details)
        , m_full_name(details->full_name())
    {
        create_module();
    }

    python_module::~python_module() {}

    void python_module::create_module()
    {
        PyObject* module = PyImport_AddModule(m_full_name.c_str());
        error::check_error();
        if (!module) {
            MGE_THROW(python::error)
                << "Failed to create Python module '" << m_full_name << "'";
        }
        m_py_module = pyobject_ref(module, pyobject_ref::incref::yes);

        std::string parent_name;
        if (!m_details || m_details->parent()->is_root()) {
            parent_name = "__main__";
        } else {
            parent_name = m_details->parent()->full_name();
        }

        pyobject_ref parent_module(PyImport_ImportModule(parent_name.c_str()));
        error::check_error();
        if (!parent_module) {
            MGE_THROW(python::error)
                << "Cannot import parent module " << parent_name;
        }

        PyObject* parent_dict = PyModule_GetDict(parent_module.get());
        error::check_error();
        if (!parent_dict) {
            MGE_THROW(python::error)
                << "Cannot get dictionary of parent module " << parent_name;
        }
        PyDict_SetItemString(parent_dict, m_full_name.c_str(), module);
        error::check_error();
    }
} // namespace mge::python