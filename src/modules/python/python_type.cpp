// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "python_type.hpp"
#include "mge/core/checked_cast.hpp"
#include "python_error.hpp"

namespace mge::python {

    struct empty_py_object
    {
        PyObject_HEAD
    };

    static PyType_Slot s_empty_slots[] = {{}};

    python_type::python_type(const mge::script::type_details& type)
        : m_type(std::const_pointer_cast<mge::script::type_details>(
              type.shared_from_this()))
        , m_python_type(nullptr)
    {
        m_spec = PyType_Spec{};
        m_spec.name = m_type->name().c_str();
        m_spec.basicsize = checked_cast<int>(sizeof(empty_py_object));
        m_spec.flags = Py_TPFLAGS_DEFAULT;
        m_spec.slots = s_empty_slots;
    }

    python_type::~python_type()
    {
        for (auto& [name, value] : m_attributes) {
            Py_CLEAR(value);
        }
        Py_CLEAR(m_python_type);
    }

    PyObject* python_type::materialize_type()
    {
        if (m_python_type) {
            return m_python_type;
        }

        PyObject* type = PyType_FromSpec(&m_spec);
        if (!type) {
            error::check_error();
        }
        for (const auto& [name, value] : m_attributes) {
            if (PyObject_SetAttrString(type, name.c_str(), value)) {
                error::check_error();
            }
        }
        m_python_type = type;
        return m_python_type;
    }

    void python_type::set_attribute(const std::string& name, PyObject* value)
    {
        m_attributes.emplace_back(std::make_tuple(name, value));
    }
#if 0
    struct complex_py_object
    {
        // clang-format off
        PyObject_HEAD
        void *object;
        // clang-format on
    };

    python_complex_type::python_complex_type(const std::string& name)
        : python_type(name)
    {
        m_spec.basicsize = checked_cast<int>(sizeof(complex_py_object));
    }

    python_complex_type::~python_complex_type() {}
#endif

} // namespace mge::python