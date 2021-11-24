#pragma once
#include "mge/script/module.hpp"
#include "python.hpp"

namespace mge::python {

    class python_module
    {
    public:
        python_module(const mge::script::module& m);
        ~python_module();

    private:
        PyObject*           m_py_module;
        mge::script::module m_module;
    };
} // namespace mge::python