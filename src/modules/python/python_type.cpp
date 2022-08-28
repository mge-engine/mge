// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "python_type.hpp"
#include "python_context.hpp"
#include "python_error.hpp"
#include "python_object_call_context.hpp"

#include "mge/core/details.hpp"
#include "mge/core/get_or_default.hpp"
#include "mge/core/gist.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/module_details.hpp"
#include "mge/script/signature.hpp"
#include "mge/script/type_details.hpp"

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {

    static PyType_Slot s_empty_slots[] = {{}};

    std::unordered_map<PyTypeObject*, python_type*> python_type::s_all_types;

    python_type::python_type(python_context&                      context,
                             const mge::script::type_details_ref& type)
        : m_python_type(nullptr)
        , m_context(context)
        , m_type(type)
    {
        m_create_data = std::make_unique<create_data>();

        m_create_data->spec = PyType_Spec{};
        m_create_data->spec.name = m_type->name().c_str();
        m_create_data->spec.flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HEAPTYPE;
        m_create_data->spec.slots = s_empty_slots;

        m_destructor.delete_ptr = nullptr;
        m_destructor.delete_shared_ptr = nullptr;
    }

    python_type::~python_type() { Py_CLEAR(m_python_type); }

    void python_type::interpreter_lost() { m_python_type = nullptr; }

    void python_type::add_enum_value(const std::string& name,
                                     int64_t            enum_value)
    {
        assert_create_data();
        m_create_data->type_attributes[name] =
            python_object(PyLong_FromLongLong(enum_value));
    }

    void python_type::add_field(const std::string&                   name,
                                const mge::script::type_details_ref& type,
                                const mge::script::invoke_function&  setter,
                                const mge::script::invoke_function&  getter)
    {
        m_fields.emplace_back(field{name, type, setter, getter, this});
    }

    void python_type::add_constructor(
        const mge::script::signature&       sig,
        const mge::script::invoke_function& new_at,
        const mge::script::invoke_function& make_shared)
    {
        MGE_DEBUG_TRACE(PYTHON)
            << "Add constructor to " << m_type->name() << ": " << gist(sig);
        m_constructors.emplace_back(&sig, &new_at, &make_shared);
    }

    void python_type::add_destructor(
        const mge::script::invoke_function& delete_ptr,
        const mge::script::invoke_function& delete_shared_ptr)
    {
        m_destructor.delete_ptr = &delete_ptr;
        m_destructor.delete_shared_ptr = &delete_shared_ptr;
    }

    void python_type::assert_create_data() const
    {
        if (!m_create_data) {
            MGE_THROW(illegal_state) << "Python type creation not in progress";
        }
    }

    PyObject* python_type::py_type() const
    {
        if (!m_python_type) {
            materialize_type();
        }
        return m_python_type;
    }

    PyObject* python_type::get_field_value(PyObject* self, void* field)
    {
        python_type::field* f = reinterpret_cast<python_type::field*>(field);
        python_object_call_context ctx(f->ptype, self);
        f->getter(ctx);
        return ctx.result();
    }

    int
    python_type::set_field_value(PyObject* self, PyObject* value, void* field)
    {
        python_type::field* f = reinterpret_cast<python_type::field*>(field);
        python_object_call_context ctx(f->ptype, self, value);
        f->setter(ctx);
        return 0;
    }

    void python_type::materialize_type() const
    {
        assert_create_data();
        if (m_type->traits().is_enum) {
            materialize_enum_type();
        } else {
            materialize_class_type();
        }
        if (m_python_type) {
            PyTypeObject* tp = reinterpret_cast<PyTypeObject*>(m_python_type);
            s_all_types[tp] = const_cast<python_type*>(this);
        }
        m_create_data.release();
    }

    void python_type::materialize_enum_type() const
    {
        auto m = m_context.get_module(m_type->module().lock());

        if (!m) {
            MGE_THROW(mge::illegal_state)
                << "Module '" << m_type->module().lock()->name()
                << "' not found";
        }

        PyTypeObject* base = &PyLong_Type;

        m_create_data->spec.basicsize = sizeof(PyLongObject);
        m_create_data->spec.flags |=
            Py_TPFLAGS_BASETYPE | Py_TPFLAGS_LONG_SUBCLASS;

        m_python_type =
            PyType_FromModuleAndSpec(m->py_module(),
                                     &m_create_data->spec,
                                     reinterpret_cast<PyObject*>(base));
        error::check_error();
        for (const auto& [name, object] : m_create_data->type_attributes) {
            auto rc = PyObject_SetAttrString(m_python_type,
                                             name.c_str(),
                                             object.borrow());
            if (rc == -1) {
                error::check_error();
            }
        }
    }

    bool python_type::is_embeddable() const
    {
        const auto t = m_type->traits();

        return !t.is_abstract && t.is_trivially_destructible;
    }

    void python_type::materialize_class_type() const
    {
        materialize_complex_class_type();
    }

    struct only_head
    {
        PyObject_HEAD;
    };

    static constexpr size_t aligned_PyObject_HEAD_size()
    {
        return ((sizeof(only_head) + 7) / 8) * 8;
    }

    void python_type::materialize_complex_class_type() const
    {
        auto m = m_context.get_module(m_type->module().lock());
        if (!m) {
            MGE_THROW(mge::illegal_state)
                << "Module '" << m_type->module().lock()->name()
                << "' not found";
        }
        for (const auto& f : m_fields) {
            PyGetSetDef getset{f.name.c_str(),
                               &get_field_value, /* getter */
                               f.setter
                                   ? &set_field_value
                                   : nullptr, /* setter, null for readonly */
                               f.name.c_str(),
                               const_cast<field*>(&f)};

            m_create_data->getset_defs.emplace_back(getset);
        }
        if (!m_create_data->getset_defs.empty()) {
            PyGetSetDef sentinel{nullptr, nullptr, nullptr, nullptr, nullptr};
            m_create_data->getset_defs.emplace_back(sentinel);
            PyType_Slot getset_slot = {Py_tp_getset,
                                       m_create_data->getset_defs.data()};
            m_create_data->slots.emplace_back(getset_slot);
        }

        if (!m_constructors.empty() && m_destructor.delete_shared_ptr) {
            PyType_Slot init_slot{Py_tp_init, &python_type::init};
            m_create_data->slots.emplace_back(init_slot);
            PyType_Slot dealloc_slot{Py_tp_dealloc, &python_type::dealloc};
            m_create_data->slots.emplace_back(dealloc_slot);
        } else {
            m_create_data->spec.flags |= Py_TPFLAGS_DISALLOW_INSTANTIATION;
        }

        if (!m_create_data->slots.empty()) {
            PyType_Slot sentinel_slot{0, nullptr};
            m_create_data->slots.emplace_back(sentinel_slot);
            m_create_data->spec.slots = m_create_data->slots.data();
        }
        m_create_data->spec.basicsize =
            static_cast<int>(aligned_PyObject_HEAD_size() + sizeof(void*));

        m_create_data->spec.flags |= Py_TPFLAGS_BASETYPE;

        m_python_type = PyType_FromModuleAndSpec(m->py_module(),
                                                 &m_create_data->spec,
                                                 nullptr);
        error::check_error();
        PyObject* python_type_ptr =
            PyLong_FromVoidPtr(const_cast<python_type*>(this));
        try {
            if (PyObject_SetAttrString(m_python_type,
                                       "_python_type_ptr",
                                       python_type_ptr)) {
                error::check_error();
            }
            Py_CLEAR(python_type_ptr);
        } catch (...) {
            Py_CLEAR(python_type_ptr);
            throw;
        }
    }

    python_type* python_type::python_type_of(PyTypeObject* tp)
    {
        return mge::get_or_default(s_all_types, tp, nullptr);
    }

    int python_type::init(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        PyTypeObject* tp = Py_TYPE(self);
        python_type*  ptp = python_type_of(tp);
        if (!ptp) {
            PyErr_SetString(
                PyExc_RuntimeError,
                "Call to __init__ for type not managed by extension");
            return 1;
        }

        return ptp->init_object(self, args, kwargs);
    }

    void python_type::dealloc(PyObject* self)
    {
        PyTypeObject* tp = Py_TYPE(self);
        python_type*  ptp = python_type_of(tp);
        if (!ptp) {
            PyErr_SetString(PyExc_RuntimeError,
                            "Cannot dealloc type not managed by extension");
        }
        if (ptp->m_destructor.delete_shared_ptr) {
            python_object_call_context context(ptp, self);
            (*(ptp->m_destructor.delete_shared_ptr))(context);
        }

        tp->tp_free(self);
        Py_DECREF(tp);
    }

    void python_type::clear_object_space(PyObject* self)
    {
        unsigned char* self_data = reinterpret_cast<unsigned char*>(self);
        self_data += aligned_PyObject_HEAD_size();
        memset(self_data, 0, sizeof(void*));
    }

    int
    python_type::init_object(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        clear_object_space(self);
        if (kwargs) {
            PyErr_SetString(PyExc_RuntimeError,
                            "Unsupported use of keyword arguments");
            return 1;
        }

        if (!PyTuple_Check(args)) {
            PyErr_SetString(PyExc_RuntimeError,
                            "Argument to init function must be a tuple");
            return 1;
        }

        if (PyTuple_Size(args) == 0) {
            for (const auto& ctor : m_constructors) {
                if (ctor.sig->empty()) {
                    return init_default(ctor, self);
                }
            }
            PyErr_SetString(
                PyExc_RuntimeError,
                "No matching constructor for empty argument list found");
            return 1;
        } else {
            PyErr_SetString(PyExc_NotImplementedError,
                            "Constructor with arguments");
            return 1;
        }

        return 0;
    }

    int python_type::init_default(const python_type::constructor& ctor,
                                  PyObject*                       self)
    {
        python_object_call_context ctx(this, self);
        (*ctor.new_shared)(ctx);
        return 0;
    }

    void* python_type::this_ptr(PyObject* self)
    {
        if (!self) {
            return nullptr;
        }

        unsigned char* self_data = reinterpret_cast<unsigned char*>(self);
        self_data += aligned_PyObject_HEAD_size();
        std::shared_ptr<int>** sptr =
            reinterpret_cast<std::shared_ptr<int>**>(self_data);
        if (*sptr)
            return (*sptr)->get();
        else {
            return nullptr;
        }
    }

    void* python_type::shared_ptr_address(PyObject* self) const
    {
        unsigned char* self_data = reinterpret_cast<unsigned char*>(self);
        self_data += aligned_PyObject_HEAD_size();
        return self_data;
    }

} // namespace mge::python