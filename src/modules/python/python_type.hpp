// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/memory.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/type_details.hpp"

#include "python.hpp"
#include "python_object.hpp"

#include <map>
#include <string>
#include <vector>

namespace mge::python {

    MGE_DECLARE_REF(python_type);

    class python_context;

    class python_type
    {
    private:
        struct create_data
        {
            PyType_Spec                          spec;
            std::vector<PyType_Slot>             slots;
            std::map<std::string, python_object> type_attributes;
        };

    public:
        python_type(python_context&                      context,
                    const mge::script::type_details_ref& type);

        ~python_type();

        void add_enum_value(const std::string& name, int64_t value);
        const std::string& local_name() const { return m_type->name(); }
        PyObject*          py_type() const;

    private:
        void assert_create_data() const;
        void materialize_type() const;
        void materialize_enum_type() const;
        void materialize_class_type() const;

        mutable std::unique_ptr<create_data> m_create_data;
        mutable PyObject*                    m_python_type;
        python_context&                      m_context;
        mge::script::type_details_ref        m_type;
    };
} // namespace mge::python