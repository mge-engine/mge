// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "python_context.hpp"

#include "mge/core/trace.hpp"

#include "mge/reflection/module.hpp"
#include "mge/reflection/module_details.hpp"
#include "mge/reflection/type_details.hpp"

#include "pyobject_ref.hpp"
#include "python_binder.hpp"
#include "python_call_context.hpp"
#include "python_engine.hpp"
#include "python_error.hpp"
#include "python_instance.hpp"
#include "python_invocation_context.hpp"
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

            // If m_thread_state was already current on entry (e.g. destructor
            // called during exception unwinding from bind()), restoring 'prev'
            // would put it back as current before Delete — which crashes.
            // Swap to nullptr instead to ensure it is not current.
            PyThreadState_Swap(prev != m_thread_state ? prev : nullptr);
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

    PyObject* python_context::register_component_call(PyObject* self,
                                                      PyObject* args)
    {
        auto* ctx = static_cast<python_context*>(
            PyCapsule_GetPointer(self, "__mge_context__"));
        if (!ctx) {
            PyErr_SetString(PyExc_RuntimeError, "invalid context");
            return nullptr;
        }

        PyObject* type_obj = nullptr;
        PyObject* impl_class = nullptr;
        if (!PyArg_ParseTuple(args, "OO", &type_obj, &impl_class)) {
            return nullptr;
        }

        if (!PyType_Check(type_obj)) {
            PyErr_SetString(PyExc_TypeError,
                            "register_component arg1: expected a type object");
            return nullptr;
        }
        if (!PyType_Check(impl_class)) {
            PyErr_SetString(PyExc_TypeError,
                            "register_component arg2: expected a type object");
            return nullptr;
        }

        // Extract type_details from the base type capsule
        PyObject* capsule =
            PyObject_GetAttrString(type_obj, "__mge_type_details__");
        if (!capsule || !PyCapsule_CheckExact(capsule)) {
            Py_XDECREF(capsule);
            PyErr_SetString(
                PyExc_TypeError,
                "register_component arg1: not a reflected C++ class");
            return nullptr;
        }
        auto* details = static_cast<const mge::reflection::type_details*>(
            PyCapsule_GetPointer(capsule, "__mge_type_details__"));
        Py_DECREF(capsule);

        if (!details || !details->is_class) {
            PyErr_SetString(PyExc_TypeError,
                            "register_component arg1: expected a class type");
            return nullptr;
        }

        const auto& class_details =
            std::get<mge::reflection::type_details::class_specific_details>(
                details->specific_details);

        if (!class_details.proxy_type) {
            PyErr_SetString(PyExc_TypeError,
                            "register_component arg1: type has no proxy");
            return nullptr;
        }

        // Verify impl_class is a subclass of type_obj
        if (!PyObject_IsSubclass(impl_class, type_obj)) {
            PyErr_Clear();
            PyErr_SetString(PyExc_TypeError,
                            "register_component arg2: not a subclass of arg1");
            return nullptr;
        }

        // Get implementation name from __mge_name__ or __name__
        PyObject* name_obj = PyObject_GetAttrString(impl_class, "__mge_name__");
        if (!name_obj) {
            PyErr_Clear();
            name_obj = PyObject_GetAttrString(impl_class, "__name__");
        }
        if (!name_obj || !PyUnicode_Check(name_obj)) {
            Py_XDECREF(name_obj);
            PyErr_SetString(PyExc_TypeError,
                            "register_component arg2: missing '__name__'");
            return nullptr;
        }
        std::string impl_name(PyUnicode_AsUTF8(name_obj));
        Py_DECREF(name_obj);

        std::string component_name(details->name);

        // Keep the Python impl class alive for the factory's lifetime
        auto impl_class_ref =
            std::make_shared<pyobject_ref>(impl_class,
                                           pyobject_ref::incref::yes);

        auto factory =
            [impl_class_ref]() -> std::shared_ptr<mge::component_base> {
            PyGILState_STATE g = PyGILState_Ensure();

            // Call impl_class() — goes through tp_new + tp_init_static which
            // allocates the proxy and stores it in h->object.
            PyObject* empty_tuple = PyTuple_New(0);
            PyObject* py_inst =
                PyObject_Call(impl_class_ref->get(), empty_tuple, nullptr);
            Py_DECREF(empty_tuple);
            if (!py_inst) {
                PyErr_Print();
                PyGILState_Release(g);
                MGE_THROW(mge::python::error) << "register_component factory: "
                                                 "failed to construct instance";
            }

            // inv_ctx stores a raw (non-owning) pointer to py_inst.
            // Keep py_inst alive for the proxy's lifetime via the returned
            // shared_ptr's deleter. py_inst's owning h->object keeps the
            // proxy alive in turn — no ownership cycle.
            Py_INCREF(py_inst);

            auto* h    = reinterpret_cast<python_instance_header*>(py_inst);
            auto* base = static_cast<mge::component_base*>(h->object.get());

            auto result = std::shared_ptr<mge::component_base>(
                base,
                [py_inst](mge::component_base*) { Py_DECREF(py_inst); });

            Py_DECREF(py_inst); // balance factory-local ref; result owns 1 ref

            PyGILState_Release(g);
            return result;
        };

        auto entry =
            std::make_unique<mge::dynamic_implementation_registry_entry>(
                std::move(factory),
                component_name,
                impl_name);
        ctx->m_component_entries.push_back(std::move(entry));

        Py_INCREF(Py_None);
        return Py_None;
    }

    PyObject* python_context::create_component_call(PyObject* self,
                                                    PyObject* args)
    {
        auto* ctx = static_cast<python_context*>(
            PyCapsule_GetPointer(self, "__mge_context__"));
        if (!ctx) {
            PyErr_SetString(PyExc_RuntimeError, "invalid context");
            return nullptr;
        }

        PyObject*   type_obj = nullptr;
        const char* impl_name = nullptr;
        if (!PyArg_ParseTuple(args, "Os", &type_obj, &impl_name)) {
            return nullptr;
        }

        if (!PyType_Check(type_obj)) {
            PyErr_SetString(PyExc_TypeError,
                            "create_component arg1: expected a type object");
            return nullptr;
        }

        PyObject* capsule =
            PyObject_GetAttrString(type_obj, "__mge_type_details__");
        if (!capsule || !PyCapsule_CheckExact(capsule)) {
            Py_XDECREF(capsule);
            PyErr_SetString(PyExc_TypeError,
                            "create_component arg1: not a reflected C++ class");
            return nullptr;
        }
        auto* details = static_cast<const mge::reflection::type_details*>(
            PyCapsule_GetPointer(capsule, "__mge_type_details__"));
        Py_DECREF(capsule);

        if (!details || !details->is_class) {
            PyErr_SetString(PyExc_TypeError,
                            "create_component arg1: expected a class type");
            return nullptr;
        }

        const auto& class_details =
            std::get<mge::reflection::type_details::class_specific_details>(
                details->specific_details);

        auto instance = mge::component_base::create(details->name, impl_name);
        if (!instance) {
            PyErr_Format(PyExc_RuntimeError,
                         "create_component: no implementation '%s' for '%s'",
                         impl_name,
                         std::string(details->name).c_str());
            return nullptr;
        }

        // The proxy is owned by py_inst's h->object (owning shared_ptr).
        // The returned shared_ptr's deleter keeps py_inst alive.
        // Python code holds the returned py_self reference, which owns
        // one additional ref; when it drops, tp_finalize frees the proxy.
        if (class_details.get_context) {
            void* raw_ptr = instance.get();
            auto* inv_ctx = static_cast<python_invocation_context*>(
                class_details.get_context(raw_ptr));
            if (inv_ctx && inv_ctx->self()) {
                PyObject* py_self = inv_ctx->self();
                Py_INCREF(py_self);
                return py_self;
            }
        }

        PyErr_SetString(PyExc_RuntimeError,
                        "create_component: no Python object found in context");
        return nullptr;
    }

    void python_context::bind()
    {
        // Register callable types before binding so is_python_compatible
        // recognizes them when building method dispatch tables.
        python_call_context::register_callable_type<void, uint64_t, double>();

        reflection::module root_module = reflection::module::root();
        MGE_DEBUG_TRACE(PYTHON, "Compute binding information");
        create_helper_module();

        PyThreadState* prev = PyThreadState_Swap(m_thread_state);
        try {
            python_binder b(*this);
            root_module.details()->apply(b);

            // Add mge.register_component and mge.create_component as
            // module-level functions, using a capsule to pass 'this'.
            PyObject* ctx_capsule =
                PyCapsule_New(this, "__mge_context__", nullptr);

            if (ctx_capsule) {
                const auto& mge_mod = module("::mge");
                PyObject*   py_mod = mge_mod->py_module().get();

                static PyMethodDef s_register_def{
                    "register_component",
                    &python_context::register_component_call,
                    METH_VARARGS,
                    nullptr};
                static PyMethodDef s_create_def{
                    "create_component",
                    &python_context::create_component_call,
                    METH_VARARGS,
                    nullptr};

                if (PyObject* meth =
                        PyCFunction_New(&s_register_def, ctx_capsule)) {
                    PyModule_AddObject(py_mod, "register_component", meth);
                }
                if (PyObject* meth =
                        PyCFunction_New(&s_create_def, ctx_capsule)) {
                    PyModule_AddObject(py_mod, "create_component", meth);
                }

                Py_DECREF(ctx_capsule);
            }
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
