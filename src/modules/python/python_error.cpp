// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "python_error.hpp"
#include "gil_lock.hpp"

namespace mge::python {
    MGE_DEFINE_EXCEPTION_CLASS(error);

    void error::check_status(const PyStatus& status)
    {
        gil_lock guard;
        if (PyStatus_Exception(status)) {
            MGE_THROW(mge::python::error)
                << "Python error: "
                << (status.err_msg ? status.err_msg : "(no error message)");
        }
    }

    void error::check_error()
    {
        gil_lock  guard;
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