// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "python_context.hpp"
#include "mge/core/trace.hpp"
#include "python_engine.hpp"
#include "python_error.hpp"

#include <string_view>

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {

    python_context::python_context(python_engine* engine)
        : m_engine(engine)
    {
        MGE_INFO_TRACE(PYTHON, "Python context created");
        init_interpreter();
    }

    python_context::~python_context()
    {
        if (m_thread_state) {
            PyThreadState_Clear(m_thread_state);
            PyThreadState_Delete(m_thread_state);
            m_thread_state = nullptr;
        }
    }

    void python_context::eval(const std::string& script)
    {
        MGE_DEBUG_TRACE(PYTHON, "eval");
        PyThreadState* prev = PyThreadState_Swap(m_thread_state);
        int            rc = PyRun_SimpleString(script.c_str());
        PyThreadState_Swap(prev);
        if (rc != 0) {
            MGE_THROW(mge::python::error);
        }
    }

    int python_context::main(int argc, const char** argv)
    {
        PyThreadState* prev = PyThreadState_Swap(m_thread_state);

        // Build sys.argv as a Python list
        PyObject* py_argv = PyList_New(argc);
        for (int i = 0; i < argc; ++i) {
            PyList_SetItem(py_argv, i, PyUnicode_DecodeFSDefault(argv[i]));
        }
        PySys_SetObject("argv", py_argv);
        Py_DECREF(py_argv);

        int rc = 0;
        if (argc >= 2) {
            if (std::string_view(argv[1]) == "-c" && argc >= 3) {
                rc = PyRun_SimpleString(argv[2]) == 0 ? 0 : 1;
            } else {
                FILE* f = fopen(argv[1], "r");
                if (f) {
                    rc = PyRun_SimpleFileEx(f, argv[1], 1) == 0 ? 0 : 1;
                } else {
                    rc = 1;
                }
            }
        } else {
            rc = PyRun_InteractiveLoop(stdin, "<stdin>") == 0 ? 0 : 1;
        }

        PyThreadState_Swap(prev);
        return rc;
    }

    void python_context::bind() {}

    void python_context::init_interpreter()
    {
        // Free-threaded Python does not support sub-interpreters with
        // single-phase init modules. Use a new thread state in the main
        // interpreter instead.
        PyInterpreterState* interp =
            PyThreadState_GetInterpreter(m_engine->main_thread_state());
        m_thread_state = PyThreadState_New(interp);
        if (!m_thread_state) {
            MGE_THROW(mge::illegal_state)
                << "Failed to create Python thread state";
        }
        MGE_DEBUG_TRACE(PYTHON, "Python thread state created");
    }

} // namespace mge::python
