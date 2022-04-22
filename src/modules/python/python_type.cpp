// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "python_type.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/script/type_details.hpp"

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
        m_create_data->spec.flags = Py_TPFLAGS_DEFAULT;
        m_create_data->spec.slots = s_empty_slots;
    }

    python_type::~python_type() { Py_CLEAR(m_python_type); }

    void python_type::add_enum_value(const std::string& name,
                                     int64_t            enum_value)
    {
        assert_create_data();
        m_create_data->enum_values.emplace_back(
            std::make_pair(name, enum_value));
    }

    void python_type::assert_create_data()
    {
        if (!m_create_data) {
            MGE_THROW(illegal_state) << "Python type creation not in progress";
        }
    }

} // namespace mge::python