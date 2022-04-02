#include "python_module.hpp"
#include "python_error.hpp"

namespace mge::python {
    python_module::python_module(const mge::script::module& m)
        : m_py_module(nullptr)
        , m_module(m)
    {
        if (m_module.is_root()) {
            m_py_module = PyImport_AddModule("__main__");
        } else {
            m_py_module = PyImport_AddModule(m.name().c_str());
        }
        error::check_error();
    }

    python_module::~python_module() { Py_XDECREF(m_py_module); }

} // namespace mge::python