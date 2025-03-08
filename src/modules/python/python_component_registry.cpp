// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.

#include "python_component_registry.hpp"
#include "gil_lock.hpp"
#include "mge/core/trace.hpp"
#include "python_context.hpp"
#include "python_error.hpp"
#include "python_module.hpp"

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {

    struct python_component_object
    {
        PyObject_HEAD static python_context* context;
    };

    python_context* python_component_object::context = nullptr;

    PyObject* component::register_component(PyObject* self, PyObject* args)
    {
        const char* component_name = nullptr;
        PyObject*   component_class = nullptr;
        if (!PyArg_ParseTuple(args, "sO", &component_name, &component_class)) {
            return nullptr;
        }

        if (!PyType_Check(component_class)) {
            PyErr_SetString(PyExc_TypeError, "Second argument must be a class");
            return nullptr;
        }
        MGE_DEBUG_TRACE(PYTHON)
            << "Registering component class " << component_name;

        Py_RETURN_NONE;
    }

    python_type_ref component::component_base_type(PyObject* component_class)
    {
        return python_type_ref();
    }

    static PyMethodDef python_component_methods[] = {
        {"register_component",
         component::register_component,
         METH_VARARGS | METH_STATIC,
         "Register a component"},
        {NULL} /* Sentinel */
    };

    PyTypeObject component_type = {
        // clang-format off
        PyVarObject_HEAD_INIT(nullptr, 0)
        // clang-format on
        "mge.component",                      /* tp_name */
        sizeof(python_component_object),      /* tp_basicsize */
        0,                                    /* tp_itemsize */
        0,                                    /* tp_dealloc */
        0,                                    /* tp_print */
        0,                                    /* tp_getattr */
        0,                                    /* tp_setattr */
        0,                                    /* tp_reserved */
        0,                                    /* tp_repr */
        0,                                    /* tp_as_number */
        0,                                    /* tp_as_sequence */
        0,                                    /* tp_as_mapping */
        0,                                    /* tp_hash */
        0,                                    /* tp_call */
        0,                                    /* tp_str */
        0,                                    /* tp_getattro */
        0,                                    /* tp_setattro */
        0,                                    /* tp_as_buffer */
        Py_TPFLAGS_DEFAULT,                   /* tp_flags */
        "Wrapper for component registration", /* tp_doc */
        0,                                    /* tp_traverse */
        0,                                    /* tp_clear */
        0,                                    /* tp_richcompare */
        0,                                    /* tp_weaklistoffset */
        0,                                    /* tp_iter */
        0,                                    /* tp_iternext */
        python_component_methods,             /* tp_methods */
        0,                                    /* tp_members */
        0,                                    /* tp_getset */
        0,                                    /* tp_base */
        0,                                    /* tp_dict */
        0,                                    /* tp_descr_get */
        0,                                    /* tp_descr_set */
        0,                                    /* tp_dictoffset */
        0,                                    /* tp_init */
        0,                                    /* tp_alloc */
        0,                                    /* tp_new */
        0,                                    /* tp_free */
        0,                                    /* tp_is_gc */
        0,                                    /* tp_bases */
        0,                                    /* tp_mro */
        0,                                    /* tp_cache */
        0,                                    /* tp_subclasses */
        0,                                    /* tp_weaklist */
        0,                                    /* tp_del */
        0,                                    /* tp_version_tag */
        0,                                    /* tp_finalize */
    };

    void component::create_registry_type(const python_module_ref& module,
                                         python_context*          context)
    {
        MGE_DEBUG_TRACE(PYTHON) << "Registering component type";
        gil_lock guard;
        if (PyType_Ready(&component_type) < 0) {
            error::check_error();
        }
        Py_INCREF(&component_type);
        PyObject* pymodule = module->pymodule().get();
        PyModule_AddObject(pymodule,
                           "__component__",
                           reinterpret_cast<PyObject*>(&component_type));
        error::check_error();

        python_component_object::context = context;
    }
} // namespace mge::python
