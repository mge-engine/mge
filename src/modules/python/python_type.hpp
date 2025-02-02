// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/closure.hpp"
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

        const pyobject_ref& type_object() const
        {
            return m_type_object;
        }

    private:
        void initialize();

        void init_enum();
        void init_class();
        void init_alias();
        void init_callable_class();
        void init_regular_class();
        void init_fields();
        void init_methods();
        void init_functions();
        void add_field(const std::string&                  name,
                       const mge::script::invoke_function& getter,
                       const mge::script::invoke_function& setter);
        void add_method(const std::string&                name,
                        const mge::script::type_data_ref& return_type,
                        const mge::script::type_data::call_signature& signature,
                        const mge::script::invoke_function&           method);

        void
        add_function(const std::string&                            name,
                     const mge::script::type_data_ref&             return_type,
                     const mge::script::type_data::call_signature& signature,
                     const mge::script::invoke_function&           function);

        void define_enum();
        void define_class();
        void define_callable_class();
        void define_regular_class();
        void define_alias();

        size_t select_constructor(PyObject* args) const;

        int tp_init(PyObject* self, PyObject* args, PyObject* kwds) const;

        python_context& m_context;
        std::string     m_name_in_module; // name as it appaers in the module
        std::string     m_name;           // name for type spec, qualified fully
        std::string     m_module_name;    // qualified name of the module
        PyType_Spec     m_spec{};
        pyobject_ref    m_type_object;
        std::vector<PyType_Slot>            m_type_slots;
        std::vector<PyGetSetDef>            m_type_fields;
        std::vector<PyMethodDef>            m_type_methods;
        std::map<std::string, pyobject_ref> m_attributes;
        mge::script::type_data_ref          m_type;

        struct object
        {
            // clang-format off
            PyObject_HEAD 
            void* shared_ptr_address; // std::shared_ptr<T>* shared_ptr_address
            // clang-format on
        };

        using tp_new_closure =
            mge::closure<PyObject*, PyTypeObject*, PyObject*, PyObject*>;
        using tp_dealloc_closure = mge::closure<void, PyObject*>;
        using tp_init_closure =
            mge::closure<int, PyObject*, PyObject*, PyObject*>;

        using tp_get_closure = mge::closure<PyObject*, PyObject*, void*>;
        using tp_set_closure = mge::closure<int, PyObject*, PyObject*, void*>;

        using method_closure = mge::closure<PyObject*, PyObject*, PyObject*>;
        using function_closure = mge::closure<PyObject*, PyObject*>;

        std::shared_ptr<tp_new_closure>                m_tp_new_closure;
        std::shared_ptr<tp_dealloc_closure>            m_tp_dealloc_closure;
        std::shared_ptr<tp_init_closure>               m_tp_init_closure;
        std::vector<std::shared_ptr<tp_get_closure>>   m_tp_get_closures;
        std::vector<std::shared_ptr<tp_set_closure>>   m_tp_set_closures;
        std::vector<std::shared_ptr<method_closure>>   m_method_closures;
        std::vector<std::shared_ptr<function_closure>> m_function_closures;
    };

} // namespace mge::python
