#include "python_type.hpp"
#include "python_context.hpp"
#include "python_error.hpp"
#include "python_module.hpp"


#include "mge/script/module_data.hpp"
#include "mge/script/type_data.hpp"

namespace mge::python {

    python_type::python_type(python_context&                   context,
                             const mge::script::type_data_ref& type)
        : m_context(context)
        , m_name_in_module(type->exposed_name())
        , m_type(type)
    {
        if (m_type->exposed_directly()) {
            m_name = m_module_name + "." + type->exposed_name();
            m_module_name = m_type->module().lock()->full_name();
        } else {
            m_module_name = "__mge__";
            m_name = "__mge__.";
            m_name += type->exposed_name();
        }

        if (type->is_enum()) {
            init_enum();
        }
    }

    void python_type::init_enum()
    {
        m_spec = {
            .name = m_name.c_str(),
            .basicsize = sizeof(PyLongObject),
            .flags = Py_TPFLAGS_BASETYPE | Py_TPFLAGS_LONG_SUBCLASS,
            .slots = nullptr,
        };

        for (const auto& e : m_type->enum_specific().values) {
            m_attributes[e.second] = pyobject_ref(PyLong_FromLongLong(e.first));
        }
    }

    void python_type::materialize()
    {
        if (m_type->is_enum()) {
            materialize_enum();
        }
    }

    void python_type::materialize_enum()
    {
        pyobject_ref type(PyType_FromSpec(&m_spec));
        if (!type) {
            error::check_error();
        }
    }

    void python_type::on_interpreter_lost()
    {
        for (auto& [name, attr] : m_attributes) {
            attr.reset();
        }
    }

    void python_type::on_interpreter_restore() {}

} // namespace mge::python