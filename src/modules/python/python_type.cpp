// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.

#include "python_type.hpp"
#include "python_context.hpp"
#include "python_error.hpp"
#include "python_module.hpp"

#include "mge/core/trace.hpp"
#include "mge/reflection/module_details.hpp"

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {
    python_type::python_type(python_context&                 context,
                             const reflection::type_details& details)
        : m_context(context)
        , m_type_details(details)
        , m_name(details.name)
    {
        m_module_name = details.module.lock()->full_name();

        if (details.is_enum) {
            init_enum();
        } else {
            MGE_DEBUG_TRACE(PYTHON,
                            "Creating python type for class: {}",
                            details.name);
        }
    }

    python_type::~python_type() {}

    void python_type::init_enum()
    {
        MGE_DEBUG_TRACE(PYTHON,
                        "Creating python type for enum: {}",
                        m_type_details.name);
        if (m_type_slots.empty() || m_type_slots.rbegin()->slot != 0) {
            m_type_slots.push_back({0, nullptr});
        }

        m_spec = {.name = m_name.c_str(),
                  .basicsize = sizeof(PyLongObject),
                  .flags = Py_TPFLAGS_BASETYPE | Py_TPFLAGS_LONG_SUBCLASS |
                           Py_TPFLAGS_HEAPTYPE,
                  .slots = m_type_slots.data()};
        for (const auto& [val_name, val] :
             m_type_details.enum_specific().values) {
            if (std::holds_alternative<int64_t>(val)) {
                m_attributes[std::string(val_name)] =
                    pyobject_ref(PyLong_FromLongLong(std::get<int64_t>(val)));
            } else if (std::holds_alternative<uint64_t>(val)) {
                m_attributes[std::string(val_name)] = pyobject_ref(
                    PyLong_FromUnsignedLongLong(std::get<uint64_t>(val)));
            }
        }

        python_module_ref module = m_context.module(m_module_name);
        PyObject*         base = reinterpret_cast<PyObject*>(&PyLong_Type);

        m_type_object = PyType_FromModuleAndSpec(
            m_context.module(m_module_name)->py_module().get(),
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

        if (PyModule_AddObject(module->py_module().get(),
                               m_name.c_str(),
                               m_type_object.get())) {
            error::check_error();
            MGE_THROW(python::error) << "Cannot add type '" << m_name
                                     << "' to module '" << m_module_name << "'";
        }
    }

} // namespace mge::python