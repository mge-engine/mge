// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "python_function.hpp"
#include "mge/core/trace.hpp"
#include "python_error.hpp"
#include "python_object_call_context.hpp"
#include "signature_match.hpp"
#include "value_classification.hpp"

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {

    python_function::python_function(const std::string&            name,
                                     const std::type_index&        return_type,
                                     const mge::script::signature& sig,
                                     const mge::script::invoke_function& invoke)
        : m_name(name)
    {
        m_details.push_back(details{&return_type, &sig, &invoke});
    }

    void
    python_function::add_signature(const std::type_index&        return_type,
                                   const mge::script::signature& sig,
                                   const mge::script::invoke_function& invoke)
    {
        m_details.push_back(details{&return_type, &sig, &invoke});
    }

    python_function::~python_function() {}

    PyTypeObject python_function::s_type = {
        PyVarObject_HEAD_INIT(NULL, 0) "mge.__python_function__",
        sizeof(python_function::python_function_pyobject),
        0,                                            /* tp_itemsize */
        0,                                            /* tp_dealloc */
        0,                                            /* tp_print */
        0,                                            /* tp_getattr */
        0,                                            /* tp_setattr */
        0,                                            /* tp_reserved */
        0,                                            /* tp_repr */
        0,                                            /* tp_as_number */
        0,                                            /* tp_as_sequence */
        0,                                            /* tp_as_mapping */
        0,                                            /* tp_hash  */
        0,                                            /* tp_call */
        0,                                            /* tp_str */
        0,                                            /* tp_getattro */
        0,                                            /* tp_setattro */
        0,                                            /* tp_as_buffer */
        Py_TPFLAGS_DEFAULT,                           /* tp_flags */
        "Internal type for function closure capture", /* tp_doc */
    };

    PyObject*
    python_function::call(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        if (kwargs) {
            PyErr_SetString(PyExc_RuntimeError,
                            "Unsupported use of keyword arguments");
            return nullptr;
        }

        auto py_function_self = to_function_object(self)->method;
        python_object_call_context ctx(nullptr, nullptr, args);
        size_t                     tuple_size = PyTuple_Size(args);
        mge::small_vector<value_classification, 3> value_classes;
        for (size_t i = 0; i < tuple_size; ++i) {
            value_classes.push_back(
                value_classification(PyTuple_GET_ITEM(args, i)));
        }
        auto match = best_match(py_function_self->m_details.begin(),
                                py_function_self->m_details.end(),
                                value_classes);

        if (match != py_function_self->m_details.end()) {
            (*match->invoke)(ctx);
            return ctx.result();
        } else {
            PyErr_Format(PyExc_RuntimeError,
                         "Cannot find a match for function '%s'",
                         py_function_self->m_name.c_str());
            return nullptr;
        }
    }

    void python_function::dealloc(PyObject* self)
    {
        PyTypeObject* tp = Py_TYPE(self);
        auto          m = to_function_object(self);
        m->method.reset();
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    void python_function::init(PyObject* module)
    {
        MGE_DEBUG_TRACE(PYTHON) << "Initialize internal method type";
        s_type.tp_new = PyType_GenericNew;
        s_type.tp_call = &call;
        if (PyType_Ready(&s_type)) {
            error::check_error();
        }
        Py_INCREF(&s_type);
        PyModule_AddObject(module,
                           "__python_function__",
                           reinterpret_cast<PyObject*>(&s_type));
    }

    PyObject* python_function::py_object() const
    {
        if (!m_object) {
            m_object.reset(
                PyObject_CallNoArgs(reinterpret_cast<PyObject*>(&s_type)));
            if (!m_object) {
                error::check_error();
            }
            auto mo = to_function_object(m_object.borrow());
            mo->method = const_cast<python_function*>(this)->shared_from_this();
        }
        return m_object.borrow();
    }

    void python_function::interpreter_lost() { m_object.interpreter_lost(); }

} // namespace mge::python