// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/core/closure.hpp"
#include "mge/reflection/type_details.hpp"

#include "pyobject_ref.hpp"
#include "python.hpp"
#include "python_fwd.hpp"

#include <map>
#include <memory>
#include <vector>

namespace mge::python {

    class python_type
    {
    public:
        python_type(python_context&                 context,
                    const reflection::type_details& details);
        ~python_type();

        static PyObject*
        create_foreign_instance(const mge::reflection::type_details* target_type,
                                void*                                ptr);

        static PyObject*
        create_shared_instance(const mge::reflection::type_details* element_type,
                               std::shared_ptr<void>                ptr);

    private:
        void init_enum();
        void init_class();

        static PyObject* tp_new_static(PyTypeObject* type,
                                       PyObject*     args,
                                       PyObject*     kwds);
        static int       tp_init_static(PyObject* self,
                                        PyObject* args,
                                        PyObject* kwds);
        static void      tp_finalize_static(PyObject* self);
        static PyObject* field_getter_static(PyObject* self, void* closure);
        static int       field_setter_static(PyObject* self,
                                             PyObject* value,
                                             void*     closure);

        struct field_descriptor
        {
            const mge::reflection::type_details* type;
            size_t                               field_index;
        };

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

        std::vector<std::string>                          m_string_storage;
        std::vector<field_descriptor>                     m_field_descriptors;
        std::vector<std::unique_ptr<mge::closure_base>>   m_closures;
    };

} // namespace mge::python
