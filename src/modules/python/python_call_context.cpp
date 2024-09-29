#include "python_call_context.hpp"
#include "gil_lock.hpp"
#include "python_error.hpp"

namespace mge::python {

    void python_call_context::bool_result(bool result)
    {
        gil_lock_guard guard;
        m_result = result ? Py_True : Py_False;
        Py_INCREF(m_result);
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
        gil_lock_guard guard;
        PyObject*      arg = PyTuple_GetItem(m_arguments, position);
        uint32_t       value = PyLong_AsUnsignedLong(arg);
        if (PyErr_Occurred()) {
            MGE_THROW(python::python_exception_occurred);
        }
        return value;
    }
} // namespace mge::python