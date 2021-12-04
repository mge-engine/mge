// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "python_context.hpp"
#include "mge/core/trace.hpp"
#include "python_bind_helper.hpp"
#include "python_error.hpp"
#include "python_module.hpp"

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {

    python_context::python_context()
        : m_main_module(nullptr)
        , m_main_dict(nullptr)
        , m_main_dict_copy(nullptr)
    {
        m_main_module = PyImport_AddModule("__main__");
        m_main_dict = PyModule_GetDict(m_main_module);
        m_main_dict_copy = PyDict_Copy(m_main_dict);
    }

    python_context::~python_context()
    {
        Py_CLEAR(m_main_dict_copy);
        Py_CLEAR(m_main_dict);
        Py_CLEAR(m_main_module);
    }

    void python_context::eval(const std::string& code)
    {
        PyObject* result = PyRun_String(code.c_str(),
                                        Py_file_input,
                                        m_main_dict_copy,
                                        m_main_dict_copy);
        Py_CLEAR(result);

        check_python_error();
    }

    void python_context::bind(const mge::script::module& m)
    {
        python_bind_helper helper(*this);
        helper.visit(m);
    }

    void python_context::check_python_error()
    {
        PyObject* exc = PyErr_Occurred();
        if (exc) {
            PyObject* ex_type;
            PyObject* ex_value;
            PyObject* ex_traceback;

            PyErr_Fetch(&ex_type, &ex_value, &ex_traceback);
            PyErr_NormalizeException(&ex_type, &ex_value, &ex_traceback);

            PyObject* ex_type_repr = PyObject_Str(ex_type);
            PyObject* ex_type_str =
                PyUnicode_AsEncodedString(ex_type_repr, "utf-8", "ignore");
            std::string ex_type_cstr = PyBytes_AS_STRING(ex_type_str);

            PyObject* ex_value_repr = PyObject_Str(ex_value);
            PyObject* ex_value_str =
                PyUnicode_AsEncodedString(ex_value_repr, "utf-8", "ignore");
            std::string ex_value_cstr = PyBytes_AS_STRING(ex_value_str);

            Py_XDECREF(ex_type_str);
            Py_XDECREF(ex_type_repr);
            Py_XDECREF(ex_type);
            Py_XDECREF(ex_value_repr);
            Py_XDECREF(ex_value_str);
            Py_XDECREF(ex_value);

            Py_XDECREF(ex_traceback);

            MGE_THROW(mge::python::error) << "Python error (of " << ex_type_cstr
                                          << "): " << ex_value_cstr;
        }
    }

} // namespace mge::python