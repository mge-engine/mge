// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/reflection/type_details.hpp"

#include "pyobject_ref.hpp"
#include "python.hpp"
#include "python_fwd.hpp"

#include <map>
#include <vector>

namespace mge::python {

    class python_type
    {
    public:
        python_type(python_context&                 context,
                    const reflection::type_details& details);
        ~python_type();

    private:
        void init_enum();
        void init_class();

        python_context&                 m_context;
        const reflection::type_details& m_type_details;
        std::string                     m_name;
        std::string                     m_module_name;

        pyobject_ref                        m_type_object;
        PyType_Spec                         m_spec{};
        std::vector<PyType_Slot>            m_type_slots;
        std::vector<PyGetSetDef>            m_type_fields;
        std::vector<PyMethodDef>            m_type_methods;
        std::map<std::string, pyobject_ref> m_attributes;
    };

} // namespace mge::python