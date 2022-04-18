// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/memory.hpp"
#include "mge/script/script_fwd.hpp"
#include "python.hpp"

#include <vector>

namespace mge::python {

    MGE_DECLARE_REF(python_type);

    class python_context;

    class python_type
    {
    protected:
        python_type(python_context&                      context,
                    const mge::script::type_details_ref& type);

    public:
        virtual ~python_type();

        static python_type_ref
        make_python_type(python_context&                      context,
                         const mge::script::type_details_ref& type);

    protected:
        PyType_Spec                   m_spec;
        std::vector<PyType_Slot>      m_slots;
        PyObject*                     m_python_type;
        python_context&               m_context;
        mge::script::type_details_ref m_type;
    };
} // namespace mge::python