// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "python_error.hpp"
#include "gil_lock.hpp"

namespace mge::python {
    MGE_DEFINE_EXCEPTION_CLASS(error);

    void error::check_status(const PyStatus& status)
    {
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
            PyObject* ex = PyErr_GetRaisedException();

            PyObject* ex_type = PyObject_Type(ex);
            PyObject* ex_type_repr = PyObject_Str(ex_type);
            PyObject* ex_type_str =
                PyUnicode_AsEncodedString(ex_type_repr, "utf-8", "ignore");
            std::string ex_type_cstr =
                ex_type_str ? PyBytes_AS_STRING(ex_type_str) : "(unknown type)";

            PyObject* ex_value_repr = PyObject_Str(ex);
            PyObject* ex_value_str =
                PyUnicode_AsEncodedString(ex_value_repr, "utf-8", "ignore");
            std::string ex_value_cstr =
                ex_value_str ? PyBytes_AS_STRING(ex_value_str) : "(no message)";

            Py_XDECREF(ex_type_str);
            Py_XDECREF(ex_type_repr);
            Py_XDECREF(ex_type);
            Py_XDECREF(ex_value_repr);
            Py_XDECREF(ex_value_str);
            Py_DECREF(ex);

            MGE_THROW(mge::python::error) << "Python error (of " << ex_type_cstr
                                          << "): " << ex_value_cstr;
        }
    }

} // namespace mge::python