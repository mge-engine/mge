// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/memory.hpp"
#include "mge/script/field_details.hpp"
#include "mge/script/type_details.hpp"
#include "python.hpp"
#include <functional>
#include <string>
#include <vector>

namespace mge::python {

    class python_context;
    MGE_DECLARE_REF(python_type);
    MGE_DECLARE_REF(python_complex_type);

    class python_type
    {
    public:
        python_type(python_context&                  context,
                    const mge::script::type_details& type);
        virtual ~python_type();

        PyObject* materialize_type();

        void set_attribute(const std::string& name, PyObject* value);

        std::string full_name() const { return m_type->full_name(); }
        const mge::script::type_details& details() { return *m_type; }

        python_context& context() const { return *m_python_context; }

    protected:
        virtual void prepare_materialize();

        PyType_Spec                   m_spec;
        mge::script::type_details_ref m_type;
        PyObject*                     m_python_type;
        python_context*               m_python_context;

    private:
        typedef std::tuple<std::string, PyObject*> attr_value;
        std::vector<attr_value>                    m_attributes;
    };

    class python_complex_type : public python_type
    {
    public:
        python_complex_type(python_context&                  context,
                            const mge::script::type_details& type);
        virtual ~python_complex_type();

        void add_field(const mge::script::field_details& f);

    protected:
        void prepare_materialize() override;

    private:
        std::vector<PyType_Slot> m_slots;
        std::vector<PyGetSetDef> m_getsetdefs;
        std::vector<std::reference_wrapper<const mge::script::field_details>>
            m_fields;
    };

} // namespace mge::python