// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/memory.hpp"
#include "python.hpp"
#include <string>
#include <vector>

namespace mge::python {

    MGE_DECLARE_REF(python_type);

    class python_type
    {
    public:
        python_type(const std::string& name, size_t size);
        ~python_type();

        PyObject* materialize_type();

        void set_attribute(const std::string& name, PyObject* value);

    private:
        typedef std::tuple<std::string, PyObject*> attr_value;
        std::string                                m_name;
        PyType_Spec                                m_spec;
        std::vector<attr_value>                    m_attributes;
    };

} // namespace mge::python