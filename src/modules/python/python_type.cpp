// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "python_type.hpp"
#include "python_context.hpp"
#include "python_error.hpp"

#include "mge/core/details.hpp"
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

    void python_type::interpreter_lost() { m_python_type = nullptr; }

    void python_type::add_enum_value(const std::string& name,
                                     int64_t            enum_value)
    {
        assert_create_data();
        m_create_data->type_attributes[name] =
            python_object(PyLong_FromLongLong(enum_value));
    }

    void python_type::add_field(const std::string&                   name,
                                const mge::script::type_details_ref& type,
                                const mge::script::invoke_function&  setter,
                                const mge::script::invoke_function&  getter)
    {
        m_fields.emplace_back(field{name, type, setter, getter});
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

    PyObject* python_type::get_field_value(PyObject* self, void* field)
    {
        return nullptr;
    }

    int
    python_type::set_field_value(PyObject* self, PyObject* value, void* field)
    {
        return 0;
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

    bool python_type::is_embeddable() const
    {
        const auto t = m_type->traits();

        return !t.is_abstract && t.is_trivially_destructible;
    }

    void python_type::materialize_class_type() const
    {
        materialize_complex_class_type();
    }

    struct only_head
    {
        PyObject_HEAD;
    };

    static constexpr size_t aligned_PyObject_HEAD_size()
    {
        return ((sizeof(only_head) + 7) / 8) * 8;
    }

    void python_type::materialize_complex_class_type() const
    {
        auto m = m_context.get_module(m_type->module().lock());
        if (!m) {
            MGE_THROW(mge::illegal_state)
                << "Module '" << m_type->module().lock()->name()
                << "' not found";
        }
        for (const auto& f : m_fields) {
            PyGetSetDef getset{f.name.c_str(),
                               &get_field_value, /* getter */
                               f.setter
                                   ? &set_field_value
                                   : nullptr, /* setter, null for readonly */
                               f.name.c_str(),
                               const_cast<field*>(&f)};

            m_create_data->getset_defs.emplace_back(getset);
        }
        if (!m_create_data->getset_defs.empty()) {
            PyGetSetDef sentinel{nullptr, nullptr, nullptr, nullptr, nullptr};
            m_create_data->getset_defs.emplace_back(sentinel);
            PyType_Slot getset_slot = {Py_tp_getset,
                                       m_create_data->getset_defs.data()};
            m_create_data->slots.emplace_back(getset_slot);
        }

        if (!m_create_data->slots.empty()) {
            PyType_Slot sentinel_slot{0, nullptr};
            m_create_data->slots.emplace_back(sentinel_slot);
            m_create_data->spec.slots = m_create_data->slots.data();
        }
        m_create_data->spec.basicsize = static_cast<int>(
            aligned_PyObject_HEAD_size() + m_type->shared_ptr_size());

        m_create_data->spec.flags |= Py_TPFLAGS_BASETYPE;

        m_python_type = PyType_FromModuleAndSpec(m->py_module(),
                                                 &m_create_data->spec,
                                                 nullptr);
        error::check_error();
        PyObject* python_type_ptr =
            PyLong_FromVoidPtr(const_cast<python_type*>(this));
        try {
            if (PyObject_SetAttrString(m_python_type,
                                       "_python_type_ptr",
                                       python_type_ptr)) {
                error::check_error();
            }
            Py_CLEAR(python_type_ptr);
        } catch (...) {
            Py_CLEAR(python_type_ptr);
            throw;
        }
    }

} // namespace mge::python