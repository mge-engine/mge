#include "value_classification.hpp"
#include "python_type.hpp"

namespace mge::python {
    value_classification::value_classification(PyObject* obj)
        : m_type_class(TC_NONE)
    {
        if (obj) {
            if (Py_IsNone(obj))
                m_type_class = TC_NONE;
            else if (PyBool_Check(obj))
                m_type_class = TC_BOOL;
            else if (PyLong_Check(obj))
                m_type_class = TC_LONG;
            else if (PyFloat_Check(obj))
                m_type_class = TC_FLOAT;
            else if (PyUnicode_Check(obj))
                m_type_class = TC_UNICODE;
            else {
                m_type_class = TC_OBJECT;
                PyTypeObject* tp = Py_TYPE(obj);
                python_type*  ptp = python_type::python_type_of(tp);
                if (ptp != nullptr) {
                    m_type_index = ptp->type_index();
                }
            }
        }
    }

    value_classification::match_type
    value_classification::match(const std::type_index& arg_type) const
    {
        return NO_MATCH;
    }
} // namespace mge::python
