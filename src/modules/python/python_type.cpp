// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "python_type.hpp"
#include "python_context.hpp"
#include "python_error.hpp"

#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/module_details.hpp"
#include "mge/script/type_details.hpp"

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {

    static PyType_Slot s_empty_slots[] = {{}};

    python_type::python_type(python_context&                      context,
                             const mge::script::type_details_ref& type)
        : m_python_type(nullptr)
        , m_context(context)
        , m_type(type)
    {
        m_create_data = std::make_unique<create_data>();

        m_create_data->spec = PyType_Spec{};
        m_create_data->spec.name = m_type->name().c_str();
        m_create_data->spec.flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HEAPTYPE;
        m_create_data->spec.slots = s_empty_slots;
    }

    python_type::~python_type() { Py_CLEAR(m_python_type); }

    void python_type::add_enum_value(const std::string& name,
                                     int64_t            enum_value)
    {
        assert_create_data();
        m_create_data->type_attributes[name] =
            python_object(PyLong_FromLongLong(enum_value));
    }

    void python_type::assert_create_data() const
    {
        if (!m_create_data) {
            MGE_THROW(illegal_state) << "Python type creation not in progress";
        }
    }

    PyObject* python_type::py_type() const
    {
        if (!m_python_type) {
            materialize_type();
        }
        return m_python_type;
    }

    void python_type::materialize_type() const
    {
        assert_create_data();
        if (m_type->traits().is_enum) {
            materialize_enum_type();
        } else {
            materialize_class_type();
        }
        m_create_data.release();
    }

    void python_type::materialize_enum_type() const
    {
        auto m = m_context.get_module(m_type->module().lock());

        if (!m) {
            MGE_THROW(mge::illegal_state)
                << "Module '" << m_type->module().lock()->name()
                << "' not found";
        }

        PyTypeObject* base = &PyLong_Type;

        m_create_data->spec.basicsize = sizeof(PyLongObject);
        m_create_data->spec.flags |=
            Py_TPFLAGS_BASETYPE | Py_TPFLAGS_LONG_SUBCLASS;

        m_python_type =
            PyType_FromModuleAndSpec(m->py_module(),
                                     &m_create_data->spec,
                                     reinterpret_cast<PyObject*>(base));
        error::check_error();
        for (const auto& [name, object] : m_create_data->type_attributes) {
            auto rc = PyObject_SetAttrString(m_python_type,
                                             name.c_str(),
                                             object.borrow());
            if (rc == -1) {
                error::check_error();
            }
        }
    }

    void python_type::materialize_class_type() const {}

} // namespace mge::python