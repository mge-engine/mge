// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/memory.hpp"
#include "mge/script/field_details.hpp"
#include "python.hpp"
#include <string>
#include <vector>

namespace mge::python {

    MGE_DECLARE_REF(python_type);
    MGE_DECLARE_REF(python_complex_type);

    class python_type
    {
    public:
        python_type(const std::string& name);
        virtual ~python_type();

        PyObject* materialize_type();

        void set_attribute(const std::string& name, PyObject* value);

    protected:
        PyType_Spec m_spec;

    private:
        typedef std::tuple<std::string, PyObject*> attr_value;
        std::string                                m_name;
        std::vector<attr_value>                    m_attributes;
    };

    class python_complex_type : public python_type
    {
    public:
        python_complex_type(const std::string& name);
        virtual ~python_complex_type();
    };

} // namespace mge::python