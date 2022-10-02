#include "python_method.hpp"
#include "mge/core/trace.hpp"
#include "python_error.hpp"
#include "python_object_call_context.hpp"

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {

    python_method::python_method(const python_type_ref&        type,
                                 const std::string&            name,
                                 const std::type_index&        return_type,
                                 const mge::script::signature& sig,
                                 const mge::script::invoke_function& invoke)
        : m_type(type)
        , m_name(name)
        , m_return_type(return_type)
        , m_signature(sig)
        , m_invoke(invoke)
    {}

    python_method::~python_method() {}

    PyTypeObject python_method::s_type = {
        PyVarObject_HEAD_INIT(NULL, 0) "mge.__python_method__",
        sizeof(python_method::python_method_pyobject),
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
    python_method::call(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        auto py_method_self = to_method_object(self)->method;
        python_object_call_context ctx(py_method_self->m_type.get(),
                                       PyTuple_GetItem(args, 1),
                                       args,
                                       1);
        py_method_self->m_invoke(ctx);
        return ctx.result();
    }

    void python_method::dealloc(PyObject* self)
    {
        PyTypeObject* tp = Py_TYPE(self);
        auto          m = to_method_object(self);
        m->method.reset();
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    void python_method::init(PyObject* module)
    {
        MGE_DEBUG_TRACE(PYTHON) << "Init internal method type";
        s_type.tp_new = PyType_GenericNew;
        s_type.tp_call = &call;
        if (PyType_Ready(&s_type)) {
            error::check_error();
        }
        Py_INCREF(&s_type);
        PyModule_AddObject(module,
                           "__python_method__",
                           reinterpret_cast<PyObject*>(&s_type));
    }

    PyObject* python_method::py_object() const
    {
        if (!m_object) {
            m_object.reset(
                PyObject_CallNoArgs(reinterpret_cast<PyObject*>(&s_type)));
            if (!m_object) {
                error::check_error();
            }
            auto mo = to_method_object(m_object.borrow());
            mo->method = const_cast<python_method*>(this)->shared_from_this();
        }
        return m_object.borrow();
    }

    void python_method::interpreter_lost() { m_object.interpreter_lost(); }

} // namespace mge::python