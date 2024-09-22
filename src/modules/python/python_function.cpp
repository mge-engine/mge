#include "python_function.hpp"
#include "mge/script/function_data.hpp"
#include "mge/script/module_data.hpp"
#include "python_error.hpp"
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

    PyTypeObject python_function::s_type = {
        // clang-format off
        PyVarObject_HEAD_INIT(nullptr, 0)
        // clang-format on
        "__mge__.__function__",         /* tp_name */
        sizeof(python_function_object), /* tp_basicsize */
        0,                              /* tp_itemsize */
        0,                              /* tp_dealloc */
        0,                              /* tp_print */
        0,                              /* tp_getattr */
        0,                              /* tp_setattr */
        0,                              /* tp_reserved */
        0,                              /* tp_repr */
        0,                              /* tp_as_number */
        0,                              /* tp_as_sequence */
        0,                              /* tp_as_mapping */
        0,                              /* tp_hash */
        0,                              /* tp_call */
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
        0,                              /* tp_methods */
        0,                              /* tp_members */
        0,                              /* tp_getset */
        0,                              /* tp_base */
        0,                              /* tp_dict */
        0,                              /* tp_descr_get */
        0,                              /* tp_descr_set */
        0,                              /* tp_dictoffset */
        0,                              /* tp_init */
        0,                              /* tp_alloc */
        0,                              /* tp_new */
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

    void python_function::register_function_type(PyObject* module)
    {
        if (PyType_Ready(&s_type) < 0) {
            error::check_error();
        }

        Py_INCREF(&s_type);
        PyModule_AddObject(module,
                           "__function__",
                           reinterpret_cast<PyObject*>(&s_type));
        error::check_error();
    }

} // namespace mge::python