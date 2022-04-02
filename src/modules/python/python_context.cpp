// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "python_context.hpp"
#include "mge/core/trace.hpp"

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {

    python_context::python_context()
        : m_main_module(nullptr)
        , m_main_dict(nullptr)
        , m_main_dict_copy(nullptr)
    {
        m_main_module = PyImport_AddModule("__main__");
        m_main_dict = PyModule_GetDict(m_main_module);
        m_main_dict_copy = PyDict_Copy(m_main_dict);
    }

    python_context::~python_context()
    {
        Py_CLEAR(m_main_dict_copy);
        Py_CLEAR(m_main_dict);
        Py_CLEAR(m_main_module);
    }

    void python_context::eval(const std::string& code)
    {
        PyObject* result = PyRun_String(code.c_str(),
                                        Py_file_input,
                                        m_main_dict_copy,
                                        m_main_dict_copy);
        Py_CLEAR(result);
    }

    void python_context::bind(const mge::script::module& m)
    {
        // python_binder b(*this)
        // b.bind(m)
    }

    int python_context::main(int argc, const char** argv)
    {
        return Py_BytesMain(argc, const_cast<char**>(argv));
    }

} // namespace mge::python
