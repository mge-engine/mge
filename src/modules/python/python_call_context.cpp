#include "python_call_context.hpp"
#include "gil_lock.hpp"
#include "python_error.hpp"

#include "mge/core/checked_cast.hpp"
#include "mge/core/exception.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge::python {

    void python_call_context::bool_result(bool result)
    {
        gil_lock guard;
        m_result = result ? Py_True : Py_False;
        Py_INCREF(m_result);
    }

    void python_call_context::int8_t_result(int8_t result)
    {
        gil_lock guard;
        m_result = PyLong_FromLong(result);
    }

    void python_call_context::uint8_t_result(uint8_t result)
    {
        gil_lock guard;
        m_result = PyLong_FromUnsignedLong(result);
    }

    void python_call_context::int16_t_result(int16_t result)
    {
        gil_lock guard;
        m_result = PyLong_FromLong(result);
    }

    void python_call_context::uint16_t_result(uint16_t result)
    {
        gil_lock guard;
        m_result = PyLong_FromUnsignedLong(result);
    }

    void python_call_context::int32_t_result(int32_t result)
    {
        gil_lock guard;
        m_result = PyLong_FromLong(result);
    }

    void python_call_context::uint32_t_result(uint32_t result)
    {
        gil_lock guard;
        m_result = PyLong_FromUnsignedLong(result);
    }

    void python_call_context::int64_t_result(int64_t result)
    {
        gil_lock guard;
        m_result = PyLong_FromLongLong(result);
    }

    void python_call_context::uint64_t_result(uint64_t result)
    {
        gil_lock guard;
        m_result = PyLong_FromUnsignedLongLong(result);
    }

    void python_call_context::float_result(float result)
    {
        gil_lock guard;
        m_result = PyFloat_FromDouble(result);
    }

    void python_call_context::double_result(double result)
    {
        gil_lock guard;
        m_result = PyFloat_FromDouble(result);
    }

    void python_call_context::long_double_result(long double result)
    {
        gil_lock guard;
        double   val = mge::checked_cast<double>(result);
        m_result = PyFloat_FromDouble(val);
    }

    void python_call_context::string_result(const std::string& result)
    {
        gil_lock guard;
        m_result = PyUnicode_FromString(result.c_str());
    }

    void python_call_context::wstring_result(const std::wstring& result)
    {
        gil_lock guard;
        m_result = PyUnicode_FromWideChar(result.c_str(), result.size());
    }

    void python_call_context::object_result(void* result,
                                            const mge::script::type_data_ref& t)
    {
        MGE_THROW_NOT_IMPLEMENTED << "Python object result for type: "
                                  << t->name();
    }

    void python_call_context::exception_thrown(const std::exception& e)
    {
        m_has_exception = true;
    }

    void python_call_context::exception_thrown(const mge::exception& e)
    {

        m_has_exception = true;
    }

    void python_call_context::exception_thrown() { m_has_exception = true; }

    uint32_t python_call_context::uint32_t_parameter(size_t position)
    {
        gil_lock  guard;
        PyObject* arg = PyTuple_GetItem(m_arguments, position);
        uint32_t  value = PyLong_AsUnsignedLong(arg);
        if (PyErr_Occurred()) {
            MGE_THROW(python::python_exception_occurred);
        }
        return value;
    }
} // namespace mge::python