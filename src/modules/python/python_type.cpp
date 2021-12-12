// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "python_type.hpp"
#include "mge/core/checked_cast.hpp"
#include "python_error.hpp"

namespace mge::python {

    static PyType_Slot s_empty_slots[] = {{}};

    python_type::python_type(const std::string& name, size_t size)
        : m_name(name)
    {
        m_spec = PyType_Spec{};
        m_spec.name = m_name.c_str();
        m_spec.basicsize = checked_cast<int>(size);
        m_spec.flags = Py_TPFLAGS_DEFAULT;
        m_spec.slots = s_empty_slots;
    }

    python_type::~python_type()
    {
        for (const auto& [name, value] : m_attributes) {
            Py_XDECREF(value);
        }
    }

    PyObject* python_type::materialize_type()
    {
        PyObject* type = PyType_FromSpec(&m_spec);
        if (!type) {
            error::check_error();
        }
        for (const auto& [name, value] : m_attributes) {
            if (PyObject_SetAttrString(type, name.c_str(), value)) {
                error::check_error();
            }
        }
        return type;
    }

    void python_type::set_attribute(const std::string& name, PyObject* value)
    {
        m_attributes.emplace_back(std::make_tuple(name, value));
    }

} // namespace mge::python