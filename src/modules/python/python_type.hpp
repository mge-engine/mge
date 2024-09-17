// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/memory.hpp"
#include "mge/script/type_data.hpp"

#include "pyobject_ref.hpp"
#include "python_fwd.hpp"

namespace mge::python {

    class python_type
    {
    public:
        python_type(python_context&                   context,
                    const mge::script::type_data_ref& type);
        virtual ~python_type() = default;

        void on_interpreter_loss();
        void on_interpreter_restore();
        void define_in_interpreter();

    private:
        void initialize();

        void init_enum();
        void init_class();
        void init_callable_class();
        void init_regular_class();
        void define_enum();
        void define_class();
        void define_callable_class();
        void define_regular_class();

        static PyObject*
        tp_new(PyTypeObject* subtype, PyObject* args, PyObject* kwds);
        static void tp_dealloc(PyObject* self);

        python_context& m_context;
        std::string     m_name_in_module; // name as it appaers in the module
        std::string     m_name;           // name for type spec, qualified fully
        std::string     m_module_name;    // qualified name of the module
        PyType_Spec     m_spec{};
        pyobject_ref    m_type_object;
        std::vector<PyType_Slot>            m_type_slots;
        std::map<std::string, pyobject_ref> m_attributes;
        mge::script::type_data_ref          m_type;

        struct object
        {
            // clang-format off
            PyObject_HEAD 
            void* shared_ptr_address; // std::shared_ptr<T>* shared_ptr_address
            // clang-format on
        };
    };

} // namespace mge::python
