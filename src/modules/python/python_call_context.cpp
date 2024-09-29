#include "python_call_context.hpp"

namespace mge::python {

    void python_call_context::bool_result(bool result)
    {
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

} // namespace mge::python