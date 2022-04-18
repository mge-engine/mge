// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "python_type.hpp"
#include "mge/script/type_details.hpp"

namespace mge::python {

    static PyType_Slot s_empty_slots[] = {{}};

    python_type::python_type(python_context&                      context,
                             const mge::script::type_details_ref& type)
        : m_python_type(nullptr)
        , m_context(context)
        , m_type(type)
    {
        m_spec = PyType_Spec{};
        m_spec.name = m_type->name().c_str();
        m_spec.flags = Py_TPFLAGS_DEFAULT;
        m_spec.slots = s_empty_slots;
    }

    python_type::~python_type() { Py_CLEAR(m_python_type); }

    python_type_ref
    python_type::make_python_type(python_context&                      context,
                                  const mge::script::type_details_ref& type)
    {
        if (type->traits().is_pod()) {
            python_type_ref result;
            return result;
        }

        python_type_ref result;
        return result;
    }

} // namespace mge::python