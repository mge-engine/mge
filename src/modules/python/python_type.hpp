// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/memory.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/type_details.hpp"

#include "python.hpp"
#include "python_function.hpp"
#include "python_object.hpp"

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace mge::python {

    MGE_DECLARE_REF(python_type);

    class python_context;

    class python_type : public std::enable_shared_from_this<python_type>
    {
    private:
        struct create_data
        {
            create_data() = default;
            ~create_data() = default;

            PyType_Spec                          spec;
            std::vector<PyType_Slot>             slots;
            std::map<std::string, python_object> type_attributes;
            std::vector<PyGetSetDef>             getset_defs;
            std::vector<PyMethodDef>             method_defs;
            std::vector<python_type_ref>         subtypes;
        };

    public:
        python_type(python_context&                      context,
                    const mge::script::type_details_ref& type);

        ~python_type();

        const mge::script::type_details& details() const { return *m_type; }

        void add_enum_value(const std::string& name, int64_t value);

        void add_field(const std::string&                   name,
                       const mge::script::type_details_ref& type,
                       const mge::script::invoke_function&  setter,
                       const mge::script::invoke_function&  getter);

        void add_constructor(const mge::script::signature&       sig,
                             const mge::script::invoke_function& new_at,
                             const mge::script::invoke_function& make_shared);

        void add_method(const std::string&                  name,
                        const std::type_index&              return_type,
                        const mge::script::signature&       sig,
                        const mge::script::invoke_function& invoke);

        void add_static_method(const std::string&                  name,
                               const std::type_index&              return_type,
                               const mge::script::signature&       sig,
                               const mge::script::invoke_function& invoke);

        void
        add_destructor(const mge::script::invoke_function& delete_ptr,
                       const mge::script::invoke_function& delete_shared_ptr);

        const std::string& local_name() const { return m_type->name(); }
        std::type_index    type_index() const { return m_type->type_index(); }
        bool               is_subtype() const { return m_type->is_subtype(); }
        bool               is_embeddable() const;
        PyObject*          py_type() const;
        void               interpreter_lost();

        void* this_ptr(PyObject* self);
        void* shared_ptr_address(PyObject* self) const;

        static python_type* python_type_of(PyTypeObject* tp);

        void add_type(const python_type_ref& t);

    private:
        void assert_create_data() const;
        void materialize_type() const;
        void materialize_enum_type() const;
        void materialize_class_type() const;
        void materialize_complex_class_type() const;
        void define_methods() const;

        template <size_t I>
        static PyObject* call_method(PyObject* self, PyObject* args);

        PyObject* call_method(size_t slot, PyObject* self, PyObject* args);

        static PyCFunction method_call_slot(size_t index);

        static PyObject* get_field_value(PyObject* self, void* field);
        static int
        set_field_value(PyObject* self, PyObject* value, void* field);
        static int  init(PyObject* self, PyObject* args, PyObject* kwargs);
        static void dealloc(PyObject* self);

        int  init_object(PyObject* self, PyObject* args, PyObject* kwargs);
        void clear_object_space(PyObject* self);

        void add_new_method(const std::string&                  name,
                            const std::type_index&              return_type,
                            const mge::script::signature&       sig,
                            const mge::script::invoke_function& invoke);

        void add_overloaded_method(size_t                        index,
                                   const std::string&            name,
                                   const std::type_index&        return_type,
                                   const mge::script::signature& sig,
                                   const mge::script::invoke_function& invoke);

        struct field
        {
            const std::string*                   name;
            const mge::script::type_details_ref* type;
            const mge::script::invoke_function*  getter;
            const mge::script::invoke_function*  setter;
            python_type*                         ptype;
        };

        struct constructor
        {
            const mge::script::signature*       signature;
            const mge::script::invoke_function* new_at;
            const mge::script::invoke_function* new_shared;
        };

        struct destructor
        {
            const mge::script::invoke_function* delete_ptr;
            const mge::script::invoke_function* delete_shared_ptr;
        };

        const python_type::constructor* select_constructor(PyObject* args);

        struct single_method
        {
            const std::string*                  name;
            const std::type_index*              return_type;
            const mge::script::signature*       signature;
            const mge::script::invoke_function* invoke;
        };

        struct overloaded_method
        {
            const std::string* name;

            struct variant
            {
                const std::type_index*              return_type;
                const mge::script::signature*       signature;
                const mge::script::invoke_function* invoke;
            };

            std::vector<variant> variants;
        };

        const python_type::overloaded_method::variant*
        select_method(const std::vector<overloaded_method::variant>& methods,
                      PyObject*                                      args);

        using method =
            std::variant<std::monostate, single_method, overloaded_method>;

        mutable std::unique_ptr<create_data>       m_create_data;
        mutable PyObject*                          m_python_type;
        python_context&                            m_context;
        const mge::script::type_details_ref&       m_type;
        std::vector<field>                         m_fields;
        std::map<size_t, std::vector<constructor>> m_constructors;
        destructor                                 m_destructor;

        std::vector<method>                     m_methods;
        std::unordered_map<std::string, size_t> m_method_index;

        static std::unordered_map<PyTypeObject*, python_type*> s_all_types;
    };
} // namespace mge::python