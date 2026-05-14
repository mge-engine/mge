// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "python_module.hpp"
#include "python_error.hpp"

#include "gil_lock.hpp"

namespace mge::python {
    static std::string to_python_name(const std::string& cpp_name)
    {
        std::string result = cpp_name;
        if (result.size() >= 2 && result[0] == ':' && result[1] == ':') {
            result = result.substr(2);
        }
        size_t pos = 0;
        while ((pos = result.find("::", pos)) != std::string::npos) {
            result.replace(pos, 2, ".");
            ++pos;
        }
        return result;
    }

    python_module::python_module(python_context&                       context,
                                 const reflection::module_details_ref& details)
        : m_context(context)
        , m_details(details)
        , m_full_name(details->full_name())
        , m_python_name(to_python_name(m_full_name))
    {
        create_module();
    }

    python_module::~python_module()
    {
        m_types.clear();
    }

    void python_module::create_module()
    {
        // Register module in sys.modules under its Python name (e.g. "mge")
        PyObject* module = PyImport_AddModule(m_python_name.c_str());
        error::check_error();
        if (!module) {
            MGE_THROW(python::error)
                << "Failed to create Python module '" << m_python_name << "'";
        }
        m_py_module = pyobject_ref(module, pyobject_ref::incref::yes);

        // For submodules, also register as attribute of parent module
        if (m_details && !m_details->parent()->is_root()) {
            std::string parent_python_name =
                to_python_name(m_details->parent()->full_name());
            PyObject* parent_module =
                PyImport_ImportModule(parent_python_name.c_str());
            if (parent_module) {
                std::string attr_name = m_python_name;
                size_t      dot_pos = attr_name.rfind('.');
                if (dot_pos != std::string::npos) {
                    attr_name = attr_name.substr(dot_pos + 1);
                }
                PyObject* parent_dict = PyModule_GetDict(parent_module);
                if (parent_dict) {
                    PyDict_SetItemString(parent_dict,
                                         attr_name.c_str(),
                                         module);
                }
                Py_DECREF(parent_module);
            }
        }
    }

    void python_module::add(const python_type_ref& type)
    {
        m_types.push_back(type);
    }

    void python_module::add_function(std::string                        name,
                                     std::unique_ptr<mge::closure_base> closure,
                                     PyCFunction                        fn_ptr)
    {
        auto entry      = std::make_unique<function_entry>();
        entry->name     = std::move(name);
        entry->closure  = std::move(closure);
        entry->method_def = {entry->name.c_str(), fn_ptr, METH_VARARGS, nullptr};

        PyObject* func =
            PyCFunction_NewEx(&entry->method_def, nullptr, m_py_module.get());
        if (func) {
            if (PyModule_AddObject(m_py_module.get(),
                                   entry->name.c_str(),
                                   func) < 0) {
                Py_DECREF(func);
            }
            // PyModule_AddObject steals reference on success
        }

        m_function_entries.push_back(std::move(entry));
    }

} // namespace mge::python