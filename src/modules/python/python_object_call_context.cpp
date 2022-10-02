// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "python_object_call_context.hpp"
#include "mge/core/checked_cast.hpp"
#include "mge/core/trace.hpp"
#include "python_error.hpp"
#include "python_type.hpp"

#include <numeric>

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {
    python_object_call_context::python_object_call_context(python_type* type,
                                                           PyObject*    self,
                                                           PyObject*    args,
                                                           size_t       offset)
        : m_type(type)
        , m_self(self)
        , m_args(args)
        , m_offset(offset)
        , m_result(nullptr)
        , m_args_tuple((args != nullptr) && (PyTuple_Check(args) != 0))
    {}

    void* python_object_call_context::this_ptr()
    {
        return m_type->this_ptr(m_self);
    }

    void* python_object_call_context::shared_ptr_address()
    {
        return m_type->shared_ptr_address(m_self);
    }

    bool python_object_call_context::bool_parameter(size_t position)
    {
        PyObject* obj = arg(position);

        if (obj) {
            if (PyBool_Check(obj)) {
                return obj == Py_True;
            } else if (PyLong_Check(obj)) {
                return PyLong_AsLong(obj) != 0;
            }
        }
        return false;
    }

    int8_t python_object_call_context::int8_t_parameter(size_t position)
    {
        PyObject* obj = arg(position);

        if (PyLong_Check(obj)) {
            auto l = PyLong_AsLong(obj);
            error::check_error();
            return mge::checked_cast<int8_t>(l);
        } else {
            MGE_THROW(mge::bad_cast) << "Cannot convert argument at position "
                                     << position << " to int8_t";
        }
        return 0;
    }

    uint8_t python_object_call_context::uint8_t_parameter(size_t position)
    {
        PyObject* obj = arg(position);

        if (PyLong_Check(obj)) {
            auto l = PyLong_AsLong(obj);
            error::check_error();
            return mge::checked_cast<uint8_t>(l);
        } else {
            MGE_THROW(mge::bad_cast) << "Cannot convert argument at position "
                                     << position << " to uint8_t";
        }
        return 0;
    }

    int16_t python_object_call_context::int16_t_parameter(size_t position)
    {
        PyObject* obj = arg(position);

        if (PyLong_Check(obj)) {
            auto l = PyLong_AsLong(obj);
            error::check_error();
            return mge::checked_cast<int16_t>(l);
        } else {
            MGE_THROW(mge::bad_cast) << "Cannot convert argument at position "
                                     << position << " to int16_t";
        }
        return 0;
    }

    uint16_t python_object_call_context::uint16_t_parameter(size_t position)
    {
        PyObject* obj = arg(position);

        if (PyLong_Check(obj)) {
            auto l = PyLong_AsLong(obj);
            error::check_error();
            return mge::checked_cast<uint16_t>(l);
        } else {
            MGE_THROW(mge::bad_cast) << "Cannot convert argument at position "
                                     << position << " to uint16_t";
        }
        return 0;
    }

    int32_t python_object_call_context::int32_t_parameter(size_t position)
    {
        PyObject* obj = arg(position);

        if (PyLong_Check(obj)) {
            auto l = PyLong_AsLong(obj);
            error::check_error();
            return mge::checked_cast<int32_t>(l);
        } else {
            MGE_THROW(mge::bad_cast) << "Cannot convert argument at position "
                                     << position << " to int32_t";
        }
        return 0;
    }

    uint32_t python_object_call_context::uint32_t_parameter(size_t position)
    {
        PyObject* obj = arg(position);

        if (PyLong_Check(obj)) {
            auto l = PyLong_AsLong(obj);
            error::check_error();
            return mge::checked_cast<uint32_t>(l);
        } else {
            MGE_THROW(mge::bad_cast) << "Cannot convert argument at position "
                                     << position << " to uint32_t";
        }
        return 0;
    }

    int64_t python_object_call_context::int64_t_parameter(size_t position)
    {
        PyObject* obj = arg(position);

        if (PyLong_Check(obj)) {
            auto l = PyLong_AsLong(obj);
            error::check_error();
            return l;
        } else {
            MGE_THROW(mge::bad_cast) << "Cannot convert argument at position "
                                     << position << " to int64_t";
        }
        return 0;
    }

    uint64_t python_object_call_context::uint64_t_parameter(size_t position)
    {
        PyObject* obj = arg(position);

        if (PyLong_Check(obj)) {
            auto l = PyLong_AsUnsignedLongLong(obj);
            error::check_error();
            return l;
        } else {
            MGE_THROW(mge::bad_cast) << "Cannot convert argument at position "
                                     << position << " to int64_t";
        }
        return 0;
    }

    float python_object_call_context::float_parameter(size_t position)
    {
        PyObject* obj = arg(position);

        if (PyFloat_Check(obj)) {
            auto f = PyFloat_AsDouble(obj);
            error::check_error();
            return mge::checked_cast<float>(f);
        } else {
            MGE_THROW(mge::bad_cast) << "Cannot convert argument at position "
                                     << position << " to float";
        }
    }

    double python_object_call_context::double_parameter(size_t position)
    {
        PyObject* obj = arg(position);

        if (PyFloat_Check(obj)) {
            auto d = PyFloat_AsDouble(obj);
            error::check_error();
            return d;
        } else {
            MGE_THROW(mge::bad_cast) << "Cannot convert argument at position "
                                     << position << " to double";
        }
    }

    std::string python_object_call_context::string_parameter(size_t position)
    {
        PyObject* obj = arg(position);

        if (PyUnicode_Check(obj)) {
            auto chars = PyUnicode_AsUTF8AndSize(obj, nullptr);
            error::check_error();
            return std::string(chars);
        } else {
            MGE_THROW(mge::bad_cast) << "Cannot convert argument at position "
                                     << position << " to std::string";
        }
    }

    void* python_object_call_context::object_parameter(size_t          position,
                                                       std::type_index ti)
    {

        PyObject* obj = arg(position);

        PyTypeObject* tp = Py_TYPE(obj);
        python_type*  ptp = python_type::python_type_of(tp);
        if (!ptp) {
            MGE_THROW(mge::bad_cast) << "Cannot convert argument at position "
                                     << position << " to type " << ti.name();
        }
        if (ptp->details().type_index() != ti) {
            MGE_THROW(mge::bad_cast)
                << "Cannot convert argument at position " << position
                << " from type " << ptp->details().type_index().name()
                << " to type " << ti.name();
        }

        return ptp->this_ptr(obj);
    }

    void python_object_call_context::store_bool_result(bool result)
    {
        m_result = Py_BuildValue("O", result ? Py_True : Py_False);
    }

    void python_object_call_context::store_int8_t_result(int8_t result)
    {
        m_result = Py_BuildValue("b", result);
    }

    void python_object_call_context::store_uint8_t_result(uint8_t result)
    {
        m_result = Py_BuildValue("B", result);
    }
    void python_object_call_context::store_int16_t_result(int16_t result)
    {
        m_result = Py_BuildValue("h", result);
    }
    void python_object_call_context::store_uint16_t_result(uint16_t result)
    {
        m_result = Py_BuildValue("H", result);
    }
    void python_object_call_context::store_int32_t_result(int32_t result)
    {
        m_result = Py_BuildValue("i", result);
    }
    void python_object_call_context::store_uint32_t_result(uint32_t result)
    {
        m_result = Py_BuildValue("I", result);
    }
    void python_object_call_context::store_int64_t_result(int64_t result)
    {
        m_result = Py_BuildValue("L", result);
    }
    void python_object_call_context::store_uint64_t_result(uint64_t result)
    {
        m_result = Py_BuildValue("K", result);
    }
    void python_object_call_context::store_float_result(float result)
    {
        m_result = Py_BuildValue("f", result);
    }
    void python_object_call_context::store_double_result(double result)
    {
        m_result = Py_BuildValue("d", result);
    }

    void
    python_object_call_context::store_string_result(const std::string& result)
    {
        m_result =
            Py_BuildValue("s#", result.c_str(), (Py_ssize_t)(result.size()));
    }

    PyObject* python_object_call_context::arg(size_t position)
    {
        if (m_args_tuple || position > 0) {
            auto res = PyTuple_GetItem(m_args, position + m_offset);
            error::check_error();
            return res;
        } else {
            return m_args;
        }
    }

} // namespace mge::python