#include "python_call_context.hpp"

namespace mge::python {

    void python_call_context::bool_result(bool result)
    {
        m_result = result ? Py_True : Py_False;
        Py_INCREF(m_result);
    }

} // namespace mge::python