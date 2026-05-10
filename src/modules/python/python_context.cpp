// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "python_context.hpp"

#include "mge/core/trace.hpp"

#include "mge/reflection/module.hpp"
#include "mge/reflection/module_details.hpp"

#include "python_binder.hpp"
#include "python_engine.hpp"
#include "python_error.hpp"
#include "python_module.hpp"

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
            PyThreadState* prev = PyThreadState_Swap(m_thread_state);
            Py_XDECREF(m_mge_module);
            m_mge_module = nullptr;

            m_modules.clear();

            PyThreadState_Swap(prev);
            PyThreadState_Clear(m_thread_state);
            PyThreadState_Delete(m_thread_state);
            m_thread_state = nullptr;
        }
    }

    void python_context::eval(const std::string& script)
    {
        MGE_DEBUG_TRACE(PYTHON, "eval");
        PyThreadState* prev = PyThreadState_Swap(m_thread_state);

        PyObject* main_module = PyImport_AddModule("__main__");
        PyObject* globals = PyModule_GetDict(main_module);
        PyObject* result =
            PyRun_String(script.c_str(), Py_file_input, globals, globals);

        // Capture exception info while m_thread_state is still active,
        // before PyErr_Print() can clear it or the swap loses the context.
        std::string error_msg;
        if (!result) {
            PyObject* exc = PyErr_GetRaisedException();
            if (exc) {
                PyObject* ex_type = PyObject_Type(exc);
                PyObject* ex_type_repr = PyObject_Str(ex_type);
                PyObject* ex_type_str =
                    PyUnicode_AsEncodedString(ex_type_repr, "utf-8", "ignore");
                std::string ex_type_cstr = ex_type_str
                                               ? PyBytes_AS_STRING(ex_type_str)
                                               : "(unknown type)";
                PyObject*   ex_value_repr = PyObject_Str(exc);
                PyObject*   ex_value_str =
                    PyUnicode_AsEncodedString(ex_value_repr, "utf-8", "ignore");
                std::string ex_value_cstr =
                    ex_value_str ? PyBytes_AS_STRING(ex_value_str)
                                 : "(no message)";
                Py_XDECREF(ex_type_str);
                Py_XDECREF(ex_type_repr);
                Py_XDECREF(ex_type);
                Py_XDECREF(ex_value_repr);
                Py_XDECREF(ex_value_str);
                Py_DECREF(exc);
                error_msg =
                    "Python error (of " + ex_type_cstr + "): " + ex_value_cstr;
            } else {
                error_msg = "Python error (unknown)";
            }
        }
        Py_XDECREF(result);
        PyThreadState_Swap(prev);

        if (!error_msg.empty()) {
            MGE_THROW(mge::python::error) << error_msg;
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
            } else if (std::string_view(argv[1]) == "-m" && argc >= 3) {
                // Run a module as __main__: python -m module [args...]
                // Set sys.argv to [argv[0], args after module...]
                // (alter_sys=True will replace argv[0] with module path)
                int       module_argc = argc - 2;
                PyObject* module_argv = PyList_New(module_argc);
                PyList_SetItem(module_argv,
                               0,
                               PyUnicode_DecodeFSDefault(argv[0]));
                for (int i = 1; i < module_argc; ++i) {
                    PyList_SetItem(module_argv,
                                   i,
                                   PyUnicode_DecodeFSDefault(argv[i + 2]));
                }
                PySys_SetObject("argv", module_argv);
                Py_DECREF(module_argv);

                PyObject* runpy = PyImport_ImportModule("runpy");
                if (runpy) {
                    PyObject* run_func =
                        PyObject_GetAttrString(runpy, "run_module");
                    if (run_func) {
                        PyObject* py_mod_name = PyUnicode_FromString(argv[2]);
                        PyObject* args = PyTuple_Pack(1, py_mod_name);
                        Py_DECREF(py_mod_name);
                        PyObject* kwargs = PyDict_New();
                        PyObject* py_run_name =
                            PyUnicode_FromString("__main__");
                        PyDict_SetItemString(kwargs, "run_name", py_run_name);
                        Py_DECREF(py_run_name);
                        Py_INCREF(Py_True);
                        PyDict_SetItemString(kwargs, "alter_sys", Py_True);
                        Py_DECREF(Py_True);
                        PyObject* result =
                            PyObject_Call(run_func, args, kwargs);
                        Py_DECREF(args);
                        Py_DECREF(kwargs);
                        Py_DECREF(run_func);
                        if (!result) {
                            PyErr_Print();
                            rc = 1;
                        } else {
                            Py_DECREF(result);
                        }
                    } else {
                        PyErr_Print();
                        rc = 1;
                    }
                    Py_DECREF(runpy);
                } else {
                    PyErr_Print();
                    rc = 1;
                }
            } else {
                FILE* f = fopen(argv[1], "r");
                if (f) {
                    rc = PyRun_SimpleFileEx(f, argv[1], /*closeit=*/1) == 0 ? 0
                                                                            : 1;
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

    void python_context::bind()
    {
        reflection::module root_module = reflection::module::root();
        MGE_DEBUG_TRACE(PYTHON, "Compute binding information");
        create_helper_module();

        PyThreadState* prev = PyThreadState_Swap(m_thread_state);
        try {
            python_binder b(*this);
            root_module.details()->apply(b);
        } catch (...) {
            PyThreadState_Swap(prev);
            throw;
        }
        PyThreadState_Swap(prev);
    }

    void python_context::create_helper_module()
    {
        PyThreadState* prev = PyThreadState_Swap(m_thread_state);

        // Create a module named 'mge' and register it in sys.modules
        // so it is importable as: import mge
        m_mge_module = PyModule_New("__mge__");
        if (m_mge_module) {
            PyObject* sys_modules = PyImport_GetModuleDict();
            PyDict_SetItemString(sys_modules, "__mge__", m_mge_module);
            MGE_DEBUG_TRACE(PYTHON, "Helper module '__mge__' created");
        } else {
            MGE_WARNING_TRACE(PYTHON,
                              "Failed to create helper module '__mge__'");
        }

        PyThreadState_Swap(prev);
    }

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

    void python_context::add_module(const python_module_ref& module)
    {
        m_modules[module->full_name()] = module;
    }

    const python_module_ref&
    python_context::module(const std::string& full_name) const
    {
        auto it = m_modules.find(full_name);
        if (it == m_modules.end()) {
            MGE_THROW(mge::python::error)
                << "Python module not found in context: " << full_name;
        }
        return it->second;
    }

} // namespace mge::python
