#include "python_type.hpp"
#include "gil_lock.hpp"
#include "python_context.hpp"
#include "python_error.hpp"
#include "python_module.hpp"

#include "mge/core/trace.hpp"
#include "mge/script/module_data.hpp"
#include "mge/script/type_data.hpp"

#include <mutex>

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {

    python_type::python_type(python_context&                   context,
                             const mge::script::type_data_ref& type)
        : m_context(context)
        , m_name_in_module(type->exposed_name())
        , m_type(type)
    {
        if (m_type->exposed_directly()) {
            m_module_name = m_type->module().lock()->full_name();
            m_name = m_module_name + "." + type->exposed_name();
        } else {
            m_module_name = "__mge__";
            m_name = "__mge__.";
            m_name += type->exposed_name();
        }
        MGE_DEBUG_TRACE(PYTHON) << "Full type name: " << m_name;
        initialize();
    }

    void python_type::initialize()
    {
        if (m_type->is_enum()) {
            init_enum();
        }
    }

    static PyType_Slot s_empty_slots[] = {{0, nullptr}};

    void python_type::init_enum()
    {
        std::lock_guard<gil_lock> lock(gil_lock::instance());

        m_spec = {.name = m_name.c_str(),
                  .basicsize = sizeof(PyLongObject),
                  .flags = Py_TPFLAGS_BASETYPE | Py_TPFLAGS_LONG_SUBCLASS,
                  .slots = s_empty_slots};

        for (const auto& e : m_type->enum_specific().values) {
            // overwrite on purpose for interpreter lost scenario
            m_attributes[e.second] = pyobject_ref(PyLong_FromLongLong(e.first));
        }
    } // namespace mge::python

    void python_type::define_in_interpreter()
    {
        if (m_type->is_enum()) {
            define_enum();
        }
    }

    void python_type::define_enum()
    {
        std::lock_guard<gil_lock> lock(gil_lock::instance());

        python_module_ref module = m_context.get_module(m_module_name);
        PyObject*         base = reinterpret_cast<PyObject*>(&PyLong_Type);

        m_type_object = PyType_FromModuleAndSpec(
            m_context.get_module(m_module_name)->pymodule().get(),
            &m_spec,
            base);
        if (!m_type_object) {
            error::check_error();
        }

        for (auto& [name, attr] : m_attributes) {
            if (PyObject_SetAttrString(m_type_object.get(),
                                       name.c_str(),
                                       attr.get())) {
                error::check_error();
                MGE_THROW(python::error) << "Cannot set attribute " << name;
            }
        }

        if (PyModule_AddObject(module->pymodule().get(),
                               m_name_in_module.c_str(),
                               m_type_object.get())) {
            error::check_error();
            MGE_THROW(python::error) << "Cannot add type to module";
        }
    }

    void python_type::on_interpreter_loss()
    {
        m_type_object.reset();
        for (auto& [name, attr] : m_attributes) {
            attr.reset();
        }
    }

    void python_type::on_interpreter_restore()
    {
        initialize();
        define_in_interpreter();
    }

} // namespace mge::python