// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "python_type.hpp"
#include "python_context.hpp"
#include "python_error.hpp"
#include "python_object_call_context.hpp"
#include "signature_match.hpp"
#include "value_classification.hpp"

#include "mge/core/details.hpp"
#include "mge/core/get_or_default.hpp"
#include "mge/core/gist.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/module_details.hpp"
#include "mge/script/signature.hpp"

#include "mge/script/type_details.hpp"

#include <bitset>

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {

    static PyType_Slot s_empty_slots[] = {{}};

    std::unordered_map<PyTypeObject*, python_type*> python_type::s_all_types;

    python_type::python_type(python_context&                      context,
                             const mge::script::type_details_ref& type)
        : m_create_data(std::make_unique<create_data>())
        , m_python_type(nullptr)
        , m_context(context)
        , m_type(type)
    {

        m_create_data->spec = PyType_Spec{};
        if (type->is_subtype()) {
            m_create_data->spec.name = m_type->unique_name().c_str();
        } else {
            m_create_data->spec.name = m_type->name().c_str();
        }
        m_create_data->spec.flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HEAPTYPE;
        m_create_data->spec.slots = s_empty_slots;

        m_destructor.delete_ptr = nullptr;
        m_destructor.delete_shared_ptr = nullptr;
    }

    python_type::~python_type() { Py_CLEAR(m_python_type); }

    void python_type::interpreter_lost()
    {
        m_python_type = nullptr;
        for (const auto& [_, func] : m_static_methods) {
            func->interpreter_lost();
        }
    }

    void python_type::add_enum_value(const std::string& name,
                                     int64_t            enum_value)
    {
        assert_create_data();
        m_create_data->type_attributes[name] =
            python_object(PyLong_FromLongLong(enum_value));
    }

    void python_type::add_type(const python_type_ref& t)
    {
        assert_create_data();
        m_create_data->subtypes.push_back(t);
    }

    void python_type::add_field(const std::string&                   name,
                                const mge::script::type_details_ref& type,
                                const mge::script::invoke_function&  setter,
                                const mge::script::invoke_function&  getter)
    {
        m_fields.emplace_back(field{&name, &type, &setter, &getter, this});
    }

    void python_type::add_constructor(
        const mge::script::signature&       sig,
        const mge::script::invoke_function& new_at,
        const mge::script::invoke_function& make_shared)
    {
        MGE_DEBUG_TRACE(PYTHON)
            << "Add constructor to " << m_type->name() << ": " << gist(sig);
        m_constructors[sig.size()].emplace_back(&sig, &new_at, &make_shared);
    }

    void python_type::add_method(const std::string&            name,
                                 const std::type_index&        return_type,
                                 const mge::script::signature& sig,
                                 const mge::script::invoke_function& invoke)
    {
        MGE_DEBUG_TRACE(PYTHON)
            << "Add method '" << name << "' to " << m_type->name() << ": "
            << gist(sig) << " -> " << return_type.name();

        auto it = m_method_index.find(name);
        if (it == m_method_index.end()) {
            add_new_method(name, return_type, sig, invoke);
        } else {
            add_overloaded_method(it->second, name, return_type, sig, invoke);
        }
    }

    void python_type::add_new_method(const std::string&            name,
                                     const std::type_index&        return_type,
                                     const mge::script::signature& sig,
                                     const mge::script::invoke_function& invoke)
    {
        m_methods.emplace_back(
            single_method({&name, &return_type, &sig, &invoke}));
        m_method_index[name] = m_methods.size() - 1;
    }

    void python_type::add_overloaded_method(
        size_t                              index,
        const std::string&                  name,
        const std::type_index&              return_type,
        const mge::script::signature&       sig,
        const mge::script::invoke_function& invoke)
    {

        std::visit(
            overloaded{[](std::monostate&) {
                           MGE_THROW(illegal_state)
                               << "Method definition expected";
                       },
                       [&](single_method& sm) {
                           overloaded_method om{
                               sm.name,
                               {overloaded_method::variant{sm.return_type,
                                                           sm.signature,
                                                           sm.invoke},
                                overloaded_method::variant{&return_type,
                                                           &sig,
                                                           &invoke}}};
                           m_methods[index] = om;
                       },
                       [&](overloaded_method& m) {
                           m.variants.emplace_back(overloaded_method::variant(
                               {&return_type, &sig, &invoke}));
                       }},
            m_methods[index]);

        return;
    }

    void
    python_type::add_static_method(const std::string&            name,
                                   const std::type_index&        return_type,
                                   const mge::script::signature& sig,
                                   const mge::script::invoke_function& invoke)
    {
        MGE_DEBUG_TRACE(PYTHON)
            << "Add static method '" << name << "' to " << m_type->name()
            << ": " << gist(sig) << " -> " << return_type.name();
        auto it = m_static_methods.find(name);
        if (it == m_static_methods.end()) {
            m_static_methods[name] =
                std::make_shared<python_function>(name,
                                                  return_type,
                                                  sig,
                                                  invoke);
        } else {
            it->second->add_signature(return_type, sig, invoke);
        }
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
        (*(f->getter))(ctx);
        return ctx.result();
    }

    int
    python_type::set_field_value(PyObject* self, PyObject* value, void* field)
    {
        python_type::field* f = reinterpret_cast<python_type::field*>(field);
        try {
            python_object_call_context ctx(f->ptype, self, value);
            (*(f->setter))(ctx);
        } catch (const mge::exception& ex) {
            PyErr_Format(
                PyExc_ValueError,
                "Cannot set field '%s' for type <%s> from value %S : %s",
                f->name->c_str(),
                (*f->type)->name().c_str(),
                value,
                ex.what());
            return -1;
        }
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
        python_module_ref m = m_context.get_module(m_type->enclosing_module());
        PyTypeObject*     base = &PyLong_Type;

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
        python_module_ref m = m_context.get_module(m_type->enclosing_module());

        if (!m) {
            MGE_THROW(mge::illegal_state)
                << "Module '" << m_type->module().lock()->name()
                << "' not found";
        }
        for (const auto& f : m_fields) {
            PyGetSetDef getset{f.name->c_str(),
                               &get_field_value, /* getter */
                               (*f.setter)
                                   ? &set_field_value
                                   : nullptr, /* setter, null for readonly */
                               f.name->c_str(),
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

        if (!m_methods.empty()) {
            define_methods();
            PyType_Slot method_slot{Py_tp_methods,
                                    m_create_data->method_defs.data()};
            m_create_data->slots.emplace_back(method_slot);
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

        for (const auto& subtype : m_create_data->subtypes) {
            if (PyObject_SetAttrString(m_python_type,
                                       subtype->local_name().c_str(),
                                       subtype->py_type())) {
                error::check_error();
            }
        }

        for (const auto& [name, func] : m_static_methods) {
            if (PyObject_SetAttrString(m_python_type,
                                       name.c_str(),
                                       func->py_object())) {
                error::check_error();
            }
        }
    }

    void python_type::define_methods() const
    {
        std::vector<PyMethodDef>& method_defs = m_create_data->method_defs;
        for (size_t i = 0; i < m_methods.size(); ++i) {
            std::visit(overloaded{[](const std::monostate&) {
                                      MGE_THROW(illegal_state)
                                          << "Method definition expected";
                                  },
                                  [&](const single_method& m) {
                                      PyMethodDef d{m.name->c_str(),
                                                    method_call_slot(i),
                                                    METH_VARARGS,
                                                    nullptr};
                                      method_defs.emplace_back(d);
                                  },
                                  [&](const overloaded_method& m) {
                                      PyMethodDef d{m.name->c_str(),
                                                    method_call_slot(i),
                                                    METH_VARARGS,
                                                    nullptr};
                                      method_defs.emplace_back(d);
                                  }},
                       m_methods[i]);
        }
        if (!method_defs.empty()) {
            PyMethodDef sentinel{nullptr, nullptr, 0, nullptr};
            method_defs.emplace_back(sentinel);
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

    const python_type::constructor*
    python_type::select_constructor(PyObject* args)
    {

        size_t tuple_size = PyTuple_Size(args);
        auto   it = m_constructors.find(tuple_size);
        if (it != m_constructors.end()) {
            if (it->second.size() == 1) {
                return &((it->second)[0]);
            }
            mge::small_vector<value_classification, 3> value_classes;
            for (size_t i = 0; i < tuple_size; ++i) {
                value_classes.push_back(
                    value_classification(PyTuple_GET_ITEM(args, i)));
            }
            const auto& all_ctors = it->second;

            auto found_ctor =
                best_match(all_ctors.begin(), all_ctors.end(), value_classes);
            if (found_ctor != all_ctors.end()) {
                return &(*found_ctor);
            }
        }
        return nullptr;
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

        const python_type::constructor* ctor = select_constructor(args);
        if (ctor) {
            python_object_call_context ctx(this, self, args);
            try {
                (*ctor->new_shared)(ctx);
            } catch (const mge::exception& ex) {
                PyErr_Format(PyExc_RuntimeError,
                             "Cannot create object of type <%s>: %s",
                             m_type->name().c_str(),
                             ex.what());
                return -1;
            }
            return 0;
        } else {
            PyErr_Format(PyExc_RuntimeError,
                         "No matching constructor found for type <%s>",
                         m_type->name().c_str());
            return -1;
        }
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

    const python_type::overloaded_method::variant* python_type::select_method(
        const std::vector<overloaded_method::variant>& methods, PyObject* args)
    {
        const size_t tuple_size = PyTuple_Size(args);
        mge::small_vector<value_classification, 3> value_classes;
        for (size_t i = 0; i < tuple_size; ++i) {
            value_classes.push_back(
                value_classification(PyTuple_GET_ITEM(args, i)));
        }
        auto selected =
            best_match(methods.begin(), methods.end(), value_classes);
        return selected != methods.end() ? &(*selected) : nullptr;
    }

    PyObject*
    python_type::call_method(size_t slot, PyObject* self, PyObject* args)
    {
        if (!PyTuple_Check(args)) {
            PyErr_SetString(PyExc_RuntimeError,
                            "Argument to method must be a tuple");
            return nullptr;
        }

        if (slot >= m_methods.size()) {
            PyErr_Format(PyExc_RuntimeError,
                         "Error in calling method of type '%s', method number "
                         "%zu not found",
                         m_type->name().c_str(),
                         slot);
            return nullptr;
        }

        python_object_call_context ctx(this, self, args);

        bool error_occurred = false;

        std::visit(overloaded{[&](std::monostate&) {
                                  PyErr_Format(PyExc_RuntimeError,
                                               "Method definition expected");
                                  error_occurred = true;
                              },
                              [&](single_method& m) {
                                  try {
                                      (*m.invoke)(ctx);
                                  } catch (const mge::exception& ex) {
                                      PyErr_Format(PyExc_RuntimeError,
                                                   "Error in call to %s.%s: %s",
                                                   m_type->name().c_str(),
                                                   m.name->c_str(),
                                                   ex.what());
                                      error_occurred = true;
                                  }
                              },
                              [&](overloaded_method& m) {
                                  const overloaded_method::variant* mv =
                                      select_method(m.variants, args);
                                  if (mv) {
                                      try {
                                          (*mv->invoke)(ctx);
                                      } catch (const mge::exception& ex) {
                                          PyErr_Format(
                                              PyExc_RuntimeError,
                                              "Error in call to %s.%s: %s",
                                              m_type->name().c_str(),
                                              m.name->c_str(),
                                              ex.what());
                                          error_occurred = true;
                                      }
                                  } else {
                                      PyErr_Format(
                                          PyExc_RuntimeError,
                                          "Error in call to %s.%s: Cannot find "
                                          "suitable method",
                                          m_type->name().c_str(),
                                          m.name->c_str());
                                      error_occurred = true;
                                  }
                              }},
                   m_methods[slot]);
        if (error_occurred) {
            return nullptr;
        } else {
            return ctx.result();
        }
    }

    template <size_t I>
    PyObject* python_type::call_method(PyObject* self, PyObject* args)
    {
        PyTypeObject* tp = Py_TYPE(self);
        python_type*  ptp = python_type_of(tp);
        if (!ptp) {
            PyErr_SetString(
                PyExc_RuntimeError,
                "Cannot call method on type not managed by extension");
        }
        return ptp->call_method(I, self, args);
    }

    PyCFunction python_type::method_call_slot(size_t index)
    {
        switch (index) {
        case 0:
            return &python_type::call_method<0>;
        case 1:
            return &python_type::call_method<1>;
        case 2:
            return &python_type::call_method<2>;
        case 3:
            return &python_type::call_method<3>;
        case 4:
            return &python_type::call_method<4>;
        case 5:
            return &python_type::call_method<5>;
        case 6:
            return &python_type::call_method<6>;
        case 7:
            return &python_type::call_method<7>;
        case 8:
            return &python_type::call_method<8>;
        case 9:
            return &python_type::call_method<9>;
        case 10:
            return &python_type::call_method<10>;
        case 11:
            return &python_type::call_method<11>;
        case 12:
            return &python_type::call_method<12>;
        case 13:
            return &python_type::call_method<13>;
        case 14:
            return &python_type::call_method<14>;
        case 15:
            return &python_type::call_method<15>;
        case 16:
            return &python_type::call_method<16>;
        case 17:
            return &python_type::call_method<17>;
        case 18:
            return &python_type::call_method<18>;
        case 19:
            return &python_type::call_method<19>;
        case 20:
            return &python_type::call_method<20>;
        case 21:
            return &python_type::call_method<21>;
        case 22:
            return &python_type::call_method<22>;
        case 23:
            return &python_type::call_method<23>;
        case 24:
            return &python_type::call_method<24>;
        case 25:
            return &python_type::call_method<25>;
        case 26:
            return &python_type::call_method<26>;
        case 27:
            return &python_type::call_method<27>;
        case 28:
            return &python_type::call_method<28>;
        case 29:
            return &python_type::call_method<29>;
        case 30:
            return &python_type::call_method<30>;
        case 31:
            return &python_type::call_method<31>;
        default:
            MGE_THROW(illegal_argument)
                << "Cannot retrieve method call slot function for index "
                << index;
        }
    }

} // namespace mge::python