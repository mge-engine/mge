#include "python_module.hpp"

namespace mge ::python {
    python_module::python_module(const mge::script::module& m)
        : m_py_module(nullptr)
        , m_module(m)
    {}

    python_module::~python_module() { Py_XDECREF(m_py_module); }
} // namespace mge::python