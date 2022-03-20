// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "python_type.hpp"
#include "mge/core/checked_cast.hpp"
#include "python_call_context.hpp"
#include "python_context.hpp"
#include "python_error.hpp"

namespace mge::python {

    static PyType_Slot s_empty_slots[] = {{}};

    python_type::python_type(python_context&                  context,
                             const mge::script::type_details& type)
        : m_type(std::const_pointer_cast<mge::script::type_details>(
              type.shared_from_this()))
        , m_python_type(nullptr)
        , m_python_context(&context)
    {
        m_spec = PyType_Spec{};
        m_spec.name = m_type->name().c_str();
        m_spec.basicsize = checked_cast<int>(sizeof(PyObject));
        m_spec.flags = Py_TPFLAGS_DEFAULT;
        m_spec.slots = s_empty_slots;
    }

    python_type::~python_type()
    {
        for (auto& [name, value] : m_attributes) {
            Py_CLEAR(value);
        }
        Py_CLEAR(m_python_type);
    }

    PyObject* python_type::materialize_type()
    {
        if (m_python_type) {
            return m_python_type;
        }

        prepare_materialize();

        PyObject* type = PyType_FromSpec(&m_spec);
        if (!type) {
            error::check_error();
        }
        for (const auto& [name, value] : m_attributes) {
            if (PyObject_SetAttrString(type, name.c_str(), value)) {
                error::check_error();
            }
        }
        PyObject* cpp_type_ref = PyLong_FromVoidPtr(this);
        try {
            if (PyObject_SetAttrString(type, "_cpp_type_ref", cpp_type_ref)) {
                error::check_error();
            }
            Py_CLEAR(cpp_type_ref);
        } catch (...) {
            Py_CLEAR(cpp_type_ref);
            throw;
        }
        m_python_type = type;
        return m_python_type;
    }

    void python_type::set_attribute(const std::string& name, PyObject* value)
    {
        m_attributes.emplace_back(std::make_tuple(name, value));
    }

    void python_type::prepare_materialize() {}

    struct complex_python_object
    {
        // clang-format off
        PyObject_HEAD
         python_type* python_type_cached; // or _cpp_type_ref property of ob_type;
        // clang-format on
    };

    python_complex_type::python_complex_type(
        python_context& context, const mge::script::type_details& type)
        : python_type(context, type)
    {
        m_spec.basicsize =
            checked_cast<int>(sizeof(complex_python_object) + type.type_size());
    }

    python_complex_type::~python_complex_type() {}

    static void* data_complex_object(PyObject* self)
    {
        unsigned char* ptr = reinterpret_cast<unsigned char*>((self + 1));
        ptr += sizeof(void*);
        return ptr;
    }

    static python_complex_type* cpp_type_complex_object(PyObject* self)
    {
        void** ptr = reinterpret_cast<void**>((self + 1));
        if (*ptr) {
            return reinterpret_cast<python_complex_type*>(*ptr);
        } else {
            auto pytype = reinterpret_cast<PyObject*>(self->ob_type);
            auto cpp_type_obj = PyObject_GetAttrString(pytype, "_cpp_type_ref");
            if (!cpp_type_obj) {
                error::check_error();
            }
            void* cpp_type_ptr = PyLong_AsVoidPtr(cpp_type_obj);
            Py_CLEAR(cpp_type_obj);
            *ptr = cpp_type_ptr;
            return reinterpret_cast<python_complex_type*>(*ptr);
        }
    }

    static PyObject* get_field_complex_object(PyObject* self, void* closure)
    {
        void*                             data = data_complex_object(self);
        const mge::script::field_details* field =
            reinterpret_cast<const mge::script::field_details*>(closure);
        auto type = cpp_type_complex_object(self);

        python_call_context ctx(type->context(), data);
        field->getter(ctx);

        PyObject* result = ctx.result();
        Py_INCREF(result);
        return result;
    }

    int set_field_complex_object(PyObject* self, PyObject* value, void* closure)
    {
        void*                             data = data_complex_object(self);
        const mge::script::field_details* field =
            reinterpret_cast<const mge::script::field_details*>(closure);
        auto type = cpp_type_complex_object(self);

        python_call_context ctx(type->context(), data, value);
        field->setter(ctx);
        return 0;
    }

    static void finalize_complex_object(PyObject* self)
    {
        auto                cpp_type = cpp_type_complex_object(self);
        void*               data = data_complex_object(self);
        auto                type = cpp_type_complex_object(self);
        python_call_context ctx(type->context(), data);
        cpp_type->details().destructor()(ctx);
    }

    int init_complex_type_single_ctor(PyObject* self,
                                      PyObject* args,
                                      PyObject* kwds)
    {
        void* data = data_complex_object(self);
        auto  type = cpp_type_complex_object(self);

        python_call_context ctx(type->context(), data, args);
        type->first_constructor()(ctx);

        return 0;
    }

    int
    init_complex_type_multi_ctor(PyObject* self, PyObject* args, PyObject* kwds)
    {
        void* data = data_complex_object(self);
        auto  type = cpp_type_complex_object(self);

        size_t args_count = mge::checked_cast<size_t>(PyTuple_Size(args));
        if (PyErr_Occurred()) {
            return -1;
        }

        for (const auto& [s, cf] : type->constructors()) {
            if (s.size() == args_count) {
                python_call_context ctx(type->context(), data, args);
                cf(ctx);
                return 0;
            }
        }

        return -1;
    }

    void python_complex_type::add_field(const mge::script::field_details& f)
    {
        m_fields.emplace_back(f);
    }

    void python_complex_type::add_constructor(
        const mge::script::signature&        s,
        const mge::script::context_function& cf)
    {
        m_constructors.emplace_back(s, cf);
    }

    const mge::script::context_function&
    python_complex_type::first_constructor() const
    {
        return std::get<1>(m_constructors.at(0));
    }

    void python_complex_type::prepare_materialize()
    {
        if (!m_fields.empty()) {
            for (auto& f : m_fields) {
                PyGetSetDef def = {};
                def.name = f.get().name.c_str();
                def.get = &get_field_complex_object;
                def.set = &set_field_complex_object;
                def.closure = reinterpret_cast<void*>(&f);
                m_getsetdefs.emplace_back(def);
            }
            PyGetSetDef sentinel = {};
            m_getsetdefs.emplace_back(sentinel);
            PyType_Slot fields_slot{Py_tp_getset, m_getsetdefs.data()};
            m_slots.emplace_back(fields_slot);
        }

        if (m_constructors.empty()) {
            m_spec.flags |= Py_TPFLAGS_DISALLOW_INSTANTIATION;
        } else {
            if (m_constructors.size() == 1) {
                // PyType_Slot ctor_slot{Py_tp_init,
                //                      &init_complex_type_single_ctor};
                // m_slots.emplace_back(ctor_slot);
            } else {
                // PyType_Slot ctor_slot{Py_tp_init,
                //                      &init_complex_type_multi_ctor};
                // m_slots.emplace_back(ctor_slot);
            }
        }

        if (m_type->destructor()) {
            PyType_Slot finalize_slot{Py_tp_finalize, &finalize_complex_object};
            m_slots.emplace_back(finalize_slot);
        }

        PyType_Slot slot_sentinel = {};
        if (m_slots.size() > 1) {
            m_spec.slots = m_slots.data();
        }
    }

} // namespace mge::python