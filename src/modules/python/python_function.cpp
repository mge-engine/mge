#include "python_function.hpp"
#include "mge/script/function_data.hpp"
#include "mge/script/module_data.hpp"
#include "python_error.hpp"
#include "python_module.hpp"

namespace mge::python {

    python_function::python_function(
        python_context& context, const mge::script::function_data_ref& function)
        : m_context(context)
        , m_function(function)
    {
        m_name = m_function->name();
        m_module_name = m_function->module().lock()->full_name();
    }

    void python_function::on_interpreter_loss() { m_object.release(); }

    void python_function::on_interpreter_restore() { initialize(); }

    void python_function::initialize() {}

    static PyMethodDef python_function_methods[] = {
        {NULL} /* Sentinel */
    };

    PyTypeObject python_function::s_type = {
        // clang-format off
        PyVarObject_HEAD_INIT(nullptr, 0)
        // clang-format on
        "__mge__.__function__",         /* tp_name */
        sizeof(python_function_object), /* tp_basicsize */
        0,                              /* tp_itemsize */
        &python_function::tp_dealloc,   /* tp_dealloc */
        0,                              /* tp_print */
        0,                              /* tp_getattr */
        0,                              /* tp_setattr */
        0,                              /* tp_reserved */
        0,                              /* tp_repr */
        0,                              /* tp_as_number */
        0,                              /* tp_as_sequence */
        0,                              /* tp_as_mapping */
        0,                              /* tp_hash */
        &python_function::tp_call,      /* tp_call */
        0,                              /* tp_str */
        0,                              /* tp_getattro */
        0,                              /* tp_setattro */
        0,                              /* tp_as_buffer */
        Py_TPFLAGS_DEFAULT,             /* tp_flags */
        "Wrapper for native function",  /* tp_doc */
        0,                              /* tp_traverse */
        0,                              /* tp_clear */
        0,                              /* tp_richcompare */
        0,                              /* tp_weaklistoffset */
        0,                              /* tp_iter */
        0,                              /* tp_iternext */
        python_function_methods,        /* tp_methods */
        0,                              /* tp_members */
        0,                              /* tp_getset */
        0,                              /* tp_base */
        0,                              /* tp_dict */
        0,                              /* tp_descr_get */
        0,                              /* tp_descr_set */
        0,                              /* tp_dictoffset */
        0,                              /* tp_init */
        0,                              /* tp_alloc */
        &python_function::tp_new,       /* tp_new */
        0,                              /* tp_free */
        0,                              /* tp_is_gc */
        0,                              /* tp_bases */
        0,                              /* tp_mro */
        0,                              /* tp_cache */
        0,                              /* tp_subclasses */
        0,                              /* tp_weaklist */
        0,                              /* tp_del */
        0,                              /* tp_version_tag */
        0,                              /* tp_finalize */
    };

    void python_function::tp_dealloc(PyObject* self)
    {
        python_function_object* obj =
            reinterpret_cast<python_function_object*>(self);
        obj->function.~python_function_ref();
        Py_TYPE(self)->tp_free(self);
    }

    PyObject*
    python_function::tp_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
    {
        python_function_object*                       self =
            reinterpret_cast<python_function_object*> PyObject_New(
                python_function_object,
                type);
        if (self != nullptr) {
            new (&self->function) python_function_ref();
        }
        return reinterpret_cast<PyObject*>(self);
    }

    PyObject*
    python_function::tp_call(PyObject* self, PyObject* args, PyObject* kwds)
    {
        python_function_object* obj =
            reinterpret_cast<python_function_object*>(self);
        return obj->function->call(args, kwds);
    }

    void
    python_function::register_function_type(const python_module_ref& module)
    {
        if (PyType_Ready(&s_type) < 0) {
            error::check_error();
        }
        Py_INCREF(&s_type);

        PyObject* pymodule = module->pymodule().get();
        PyModule_AddObject(pymodule,
                           "__function__",
                           reinterpret_cast<PyObject*>(&s_type));
        error::check_error();
    }

    PyObject* python_function::call(PyObject* args, PyObject* kwds)
    {
        return Py_None;
    }

} // namespace mge::python