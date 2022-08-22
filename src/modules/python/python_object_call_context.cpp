#include "python_object_call_context.hpp"
#include "mge/core/checked_cast.hpp"
#include "python_error.hpp"
#include "python_type.hpp"

#include <numeric>

namespace mge::python {
    python_object_call_context::python_object_call_context(python_type* type,
                                                           PyObject*    self,
                                                           PyObject*    args)
        : m_type(type)
        , m_self(self)
        , m_args(args)
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
        PyObject* obj = PyTuple_GetItem(m_args, position);
        error::check_error();
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
        PyObject* obj = PyTuple_GetItem(m_args, position);
        error::check_error();
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
        PyObject* obj = PyTuple_GetItem(m_args, position);
        error::check_error();
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
        PyObject* obj = PyTuple_GetItem(m_args, position);
        error::check_error();
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
        PyObject* obj = PyTuple_GetItem(m_args, position);
        error::check_error();
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
        PyObject* obj = PyTuple_GetItem(m_args, position);
        error::check_error();
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
        PyObject* obj = PyTuple_GetItem(m_args, position);
        error::check_error();
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
        PyObject* obj = PyTuple_GetItem(m_args, position);
        error::check_error();
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
        PyObject* obj = PyTuple_GetItem(m_args, position);
        error::check_error();
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
        PyObject* obj = PyTuple_GetItem(m_args, position);
        error::check_error();
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
        PyObject* obj = PyTuple_GetItem(m_args, position);
        error::check_error();
        if (PyFloat_Check(obj)) {
            auto d = PyFloat_AsDouble(obj);
            error::check_error();
            return d;
        } else {
            MGE_THROW(mge::bad_cast) << "Cannot convert argument at position "
                                     << position << " to double";
        }
    }

#if 0
        float       float_parameter(size_t position) override;
        double      double_parameter(size_t position) override;
        std::string string_parameter(size_t position) override;

        void* object_parameter(size_t position, std::type_index ti) override;

        void store_bool_result(bool result) override;
        void store_int8_t_result(int8_t result) override;
        void store_uint8_t_result(uint8_t result) override;
        void store_int16_t_result(int16_t result) override;
        void store_uint16_t_result(uint16_t result) override;
        void store_int32_t_result(int32_t result) override;
        void store_uint32_t_result(uint32_t result) override;
        void store_int64_t_result(int64_t result) override;
        void store_uint64_t_result(uint64_t result) override;
        void store_float_result(float result) override;
        void store_double_result(double result) override;
        void store_string_result(const std::string& result) override;
#endif
} // namespace mge::python