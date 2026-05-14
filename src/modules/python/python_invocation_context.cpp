// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "python_invocation_context.hpp"
#include "python_error.hpp"

namespace mge::python {

    python_invocation_context::python_invocation_context(PyObject* self)
        : m_self(self)
        , m_self_owned(false)
    {}

    python_invocation_context::~python_invocation_context()
    {
        if (m_self_owned) {
            Py_CLEAR(m_self);
        }
    }

    void python_invocation_context::keep_self_alive()
    {
        if (!m_self_owned) {
            Py_XINCREF(m_self);
            m_self_owned = true;
        }
    }

    bool python_invocation_context::call_implemented(const char* method)
    {
        // Check if the concrete Python type defines this method as a Python
        // function, not as an inherited C method_descriptor from the C++ base.
        PyObject* type_obj = reinterpret_cast<PyObject*>(Py_TYPE(m_self));
        PyObject* attr = PyObject_GetAttrString(type_obj, method);
        if (!attr) {
            PyErr_Clear();
            return false;
        }
        bool is_python_fn = PyFunction_Check(attr) != 0;
        Py_DECREF(attr);
        return is_python_fn;
    }

    void python_invocation_context::set_arg(size_t index, PyObject* obj)
    {
        if (index >= m_stored_args.size()) {
            m_stored_args.resize(index + 1);
        }
        m_stored_args[index] = pyobject_ref(obj);
    }

    void python_invocation_context::store_bool_argument(size_t index,
                                                        bool   value)
    {
        set_arg(index, PyBool_FromLong(value ? 1 : 0));
    }

    void python_invocation_context::store_int8_t_argument(size_t index,
                                                          int8_t value)
    {
        set_arg(index, PyLong_FromLong(static_cast<long>(value)));
    }

    void python_invocation_context::store_uint8_t_argument(size_t  index,
                                                           uint8_t value)
    {
        set_arg(index,
                PyLong_FromUnsignedLong(static_cast<unsigned long>(value)));
    }

    void python_invocation_context::store_int16_t_argument(size_t  index,
                                                           int16_t value)
    {
        set_arg(index, PyLong_FromLong(static_cast<long>(value)));
    }

    void python_invocation_context::store_uint16_t_argument(size_t   index,
                                                            uint16_t value)
    {
        set_arg(index,
                PyLong_FromUnsignedLong(static_cast<unsigned long>(value)));
    }

    void python_invocation_context::store_int32_t_argument(size_t  index,
                                                           int32_t value)
    {
        set_arg(index, PyLong_FromLong(static_cast<long>(value)));
    }

    void python_invocation_context::store_uint32_t_argument(size_t   index,
                                                            uint32_t value)
    {
        set_arg(index,
                PyLong_FromUnsignedLong(static_cast<unsigned long>(value)));
    }

    void python_invocation_context::store_int64_t_argument(size_t  index,
                                                           int64_t value)
    {
        set_arg(index, PyLong_FromLongLong(static_cast<long long>(value)));
    }

    void python_invocation_context::store_uint64_t_argument(size_t   index,
                                                            uint64_t value)
    {
        set_arg(index,
                PyLong_FromUnsignedLongLong(
                    static_cast<unsigned long long>(value)));
    }

    void python_invocation_context::store_float_argument(size_t index,
                                                         float  value)
    {
        set_arg(index, PyFloat_FromDouble(static_cast<double>(value)));
    }

    void python_invocation_context::store_double_argument(size_t index,
                                                          double value)
    {
        set_arg(index, PyFloat_FromDouble(value));
    }

    void
    python_invocation_context::store_long_double_argument(size_t      index,
                                                          long double value)
    {
        set_arg(index, PyFloat_FromDouble(static_cast<double>(value)));
    }

    void
    python_invocation_context::store_string_argument(size_t             index,
                                                     const std::string& value)
    {
        set_arg(
            index,
            PyUnicode_FromStringAndSize(value.data(),
                                        static_cast<Py_ssize_t>(value.size())));
    }

    mge::reflection::invocation_context::call_result_type
    python_invocation_context::call_method(const char* method)
    {
        m_result = pyobject_ref{};

        PyObject* tuple =
            PyTuple_New(static_cast<Py_ssize_t>(m_stored_args.size()));
        if (!tuple) {
            m_stored_args.clear();
            return call_result_type::CALL_FAILED;
        }

        for (size_t i = 0; i < m_stored_args.size(); ++i) {
            PyObject* arg = m_stored_args[i].get();
            if (!arg) {
                arg = Py_None;
            }
            Py_INCREF(arg);
            PyTuple_SET_ITEM(tuple, static_cast<Py_ssize_t>(i), arg);
        }
        m_stored_args.clear();

        PyObject* method_obj = PyObject_GetAttrString(m_self, method);
        if (!method_obj) {
            Py_DECREF(tuple);
            PyErr_Clear();
            return call_result_type::CALL_NOT_FOUND;
        }

        PyObject* result = PyObject_Call(method_obj, tuple, nullptr);
        Py_DECREF(method_obj);
        Py_DECREF(tuple);

        if (!result) {
            // Convert the Python exception into a C++ exception so the proxy
            // can propagate it upward without swallowing it.
            error::check_error();
            return call_result_type::CALL_FAILED; // unreachable
        }

        m_result = pyobject_ref(result);
        return call_result_type::CALL_EXECUTED;
    }

    bool python_invocation_context::get_bool_result()
    {
        return PyObject_IsTrue(m_result.get()) != 0;
    }

    int8_t python_invocation_context::get_int8_t_result()
    {
        return static_cast<int8_t>(PyLong_AsLong(m_result.get()));
    }

    uint8_t python_invocation_context::get_uint8_t_result()
    {
        return static_cast<uint8_t>(PyLong_AsUnsignedLong(m_result.get()));
    }

    int16_t python_invocation_context::get_int16_t_result()
    {
        return static_cast<int16_t>(PyLong_AsLong(m_result.get()));
    }

    uint16_t python_invocation_context::get_uint16_t_result()
    {
        return static_cast<uint16_t>(PyLong_AsUnsignedLong(m_result.get()));
    }

    int32_t python_invocation_context::get_int32_t_result()
    {
        return static_cast<int32_t>(PyLong_AsLong(m_result.get()));
    }

    uint32_t python_invocation_context::get_uint32_t_result()
    {
        return static_cast<uint32_t>(PyLong_AsUnsignedLong(m_result.get()));
    }

    int64_t python_invocation_context::get_int64_t_result()
    {
        return static_cast<int64_t>(PyLong_AsLongLong(m_result.get()));
    }

    uint64_t python_invocation_context::get_uint64_t_result()
    {
        return static_cast<uint64_t>(PyLong_AsUnsignedLongLong(m_result.get()));
    }

    float python_invocation_context::get_float_result()
    {
        return static_cast<float>(PyFloat_AsDouble(m_result.get()));
    }

    double python_invocation_context::get_double_result()
    {
        return PyFloat_AsDouble(m_result.get());
    }

    long double python_invocation_context::get_long_double_result()
    {
        return static_cast<long double>(PyFloat_AsDouble(m_result.get()));
    }

    std::string python_invocation_context::get_string_result()
    {
        Py_ssize_t  len = 0;
        const char* utf8 = PyUnicode_AsUTF8AndSize(m_result.get(), &len);
        if (!utf8) {
            PyErr_Clear();
            return {};
        }
        return std::string(utf8, static_cast<size_t>(len));
    }

} // namespace mge::python
