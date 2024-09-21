// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "python_type.hpp"
#include "gil_lock.hpp"
#include "python_call_context.hpp"
#include "python_context.hpp"
#include "python_error.hpp"
#include "python_module.hpp"

#include "mge/core/checked_cast.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/module_data.hpp"
#include "mge/script/type_data.hpp"

#include <mutex>

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {

    python_type::python_type(python_context&                   context,
                             const mge::script::type_data_ref& type)
        : m_context(context)
        , m_name_in_module(type->exposed_name())
        , m_type(type)
    {
        if (m_type->exposed_directly()) {
            m_module_name = m_type->module().lock()->full_name();
            m_name = m_module_name + "." + type->exposed_name();
        } else {
            m_module_name = "__mge__";
            m_name = "__mge__.";
            m_name += type->exposed_name();
        }
        MGE_DEBUG_TRACE(PYTHON) << "Full type name: " << m_name;
        initialize();
    }

    void python_type::initialize()
    {
        if (m_type->is_enum()) {
            init_enum();
        } else if (m_type->is_class()) {
            init_class();
        }
    }

    void python_type::init_enum()
    {
        std::lock_guard<gil_lock> lock(gil_lock::instance());
        if (m_type_slots.empty() || m_type_slots.rbegin()->slot != 0) {
            m_type_slots.push_back({0, nullptr});
        }

        m_spec = {.name = m_name.c_str(),
                  .basicsize = sizeof(PyLongObject),
                  .flags = Py_TPFLAGS_BASETYPE | Py_TPFLAGS_LONG_SUBCLASS |
                           Py_TPFLAGS_HEAPTYPE,
                  .slots = m_type_slots.data()};

        for (const auto& e : m_type->enum_specific().values) {
            // overwrite on purpose for interpreter lost scenario
            m_attributes[e.second] = pyobject_ref(PyLong_FromLongLong(e.first));
        }
    }

    void python_type::init_class()
    {
        if (m_type->is_callable()) {
            init_callable_class();
        } else {
            init_regular_class();
        }
    }

    void python_type::init_regular_class()
    {
        struct new_closure : tp_new_closure
        {
            PyObject*
            execute(PyTypeObject* subtype, PyObject* args, PyObject* kwds)
            {
                object* self;
                self = reinterpret_cast<object*>(subtype->tp_alloc(subtype, 0));
                if (self != nullptr) {
                    self->shared_ptr_address = nullptr;
                }
                return reinterpret_cast<PyObject*>(self);
            }
        };

        struct dealloc_closure : tp_dealloc_closure
        {
            dealloc_closure(const mge::script::type_data_ref& type)
                : m_type(type)
            {}

            void execute(PyObject* self)
            {
                object* obj = reinterpret_cast<object*>(self);
                if (obj->shared_ptr_address) {
                    // shared_ptr_address is the address of the shared_ptr
                    // that holds the object
                    // we need to decrement the reference count
                    // and if it is zero, delete the object
                    python_call_context ctx(nullptr, obj->shared_ptr_address);
                    m_type->class_specific().destroy_shared(ctx);
                    // and set shared_ptr_address to nullptr
                    // to avoid double deletion
                    obj->shared_ptr_address = nullptr;
                }
                Py_TYPE(self)->tp_free(self);
            }
            const mge::script::type_data_ref& m_type;
        };

        struct init_closure : tp_init_closure
        {
            init_closure(const python_type* python_type)
                : m_python_type(python_type)
            {}

            int execute(PyObject* self, PyObject* args, PyObject* kwds)
            {
                return m_python_type->tp_init(self, args, kwds);
            }
            const python_type* m_python_type;
        };

        m_tp_new_closure = std::make_shared<new_closure>();
        m_tp_dealloc_closure = std::make_shared<dealloc_closure>(m_type);
        m_tp_init_closure = std::make_shared<init_closure>(this);

        m_type_slots.emplace_back(Py_tp_new, m_tp_new_closure->function());
        if (!m_type->class_specific().constructors.empty()) {
            m_type_slots.emplace_back(Py_tp_dealloc,
                                      m_tp_dealloc_closure->function());
            m_type_slots.emplace_back(Py_tp_init,
                                      m_tp_init_closure->function());
        }
    }

    int
    python_type::tp_init(PyObject* self, PyObject* args, PyObject* kwargs) const
    {
        if (m_type->class_specific().constructors.empty()) {
            PyErr_SetString(PyExc_RuntimeError, "No constructors available");
            return -1;
        }

        // print arguments
        PyObject* repr = PyObject_Repr(args);
        if (repr) {
            MGE_DEBUG_TRACE(PYTHON) << "Arguments: " << PyUnicode_AsUTF8(repr);
            Py_DECREF(repr);
        }
        PyObject* repr_kw = PyObject_Repr(kwargs);
        if (repr_kw) {
            MGE_DEBUG_TRACE(PYTHON)
                << "Keyword arguments: " << PyUnicode_AsUTF8(repr_kw);
            Py_DECREF(repr_kw);
        }

        if (kwargs && PyDict_Check(kwargs) && PyDict_Size(kwargs) > 0) {
            PyErr_SetString(PyExc_RuntimeError,
                            "Keyword arguments not supported");
            return -1;
        }
        if (!PyTuple_Check(args)) {
            PyErr_SetString(PyExc_RuntimeError, "Arguments must be a tuple");
            return -1;
        }

        PyErr_SetString(PyExc_RuntimeError, "Not yet implemented: constructor");
        return -1;
    }

    void python_type::init_callable_class() {}

    void python_type::define_in_interpreter()
    {
        if (m_type->is_enum()) {
            define_enum();
        } else if (m_type->is_class()) {
            define_class();
        }
    }

    void python_type::define_enum()
    {
        std::lock_guard<gil_lock> lock(gil_lock::instance());

        python_module_ref module = m_context.get_module(m_module_name);
        PyObject*         base = reinterpret_cast<PyObject*>(&PyLong_Type);

        m_type_object = PyType_FromModuleAndSpec(
            m_context.get_module(m_module_name)->pymodule().get(),
            &m_spec,
            base);
        if (!m_type_object) {
            error::check_error();
        }

        for (auto& [name, attr] : m_attributes) {
            if (PyObject_SetAttrString(m_type_object.get(),
                                       name.c_str(),
                                       attr.get())) {
                error::check_error();
                MGE_THROW(python::error) << "Cannot set attribute " << name;
            }
        }

        if (PyModule_AddObject(module->pymodule().get(),
                               m_name_in_module.c_str(),
                               m_type_object.get())) {
            error::check_error();
            MGE_THROW(python::error) << "Cannot add type to module";
        }
    }

    void python_type::define_class()
    {
        if (m_type->is_callable()) {
            define_callable_class();
        } else {
            define_regular_class();
        }
    }

    void python_type::define_regular_class()
    {
        // const auto& class_specific = m_type->class_specific();

        if (m_type_slots.empty() || m_type_slots.rbegin()->slot != 0) {
            m_type_slots.push_back({0, nullptr});
        }

        m_spec = {.name = m_name.c_str(),
                  .basicsize =
                      mge::checked_cast<int>(sizeof(PyObject) + sizeof(void*)),
                  .flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HEAPTYPE,
                  .slots = m_type_slots.data()};

        std::lock_guard<gil_lock> lock(gil_lock::instance());

        python_module_ref module = m_context.get_module(m_module_name);
        m_type_object = PyType_FromModuleAndSpec(module->pymodule().get(),
                                                 &m_spec,
                                                 nullptr);
        if (!m_type_object) {
            error::check_error();
        }
        PyObject* capsule = PyCapsule_New(this, "python_type", nullptr);
        if (!capsule) {
            error::check_error();
            MGE_THROW(python::error) << "Cannot create python type capsule";
        }
        PyTypeObject* type =
            reinterpret_cast<PyTypeObject*>(m_type_object.get());
        if (PyDict_SetItemString(type->tp_dict,
                                 "__mge__python_type__",
                                 capsule)) {
            Py_DECREF(capsule);
            error::check_error();
            MGE_THROW(python::error)
                << "Cannot set type attribute for python type";
        }
        Py_DECREF(capsule);

        if (PyModule_AddObject(module->pymodule().get(),
                               m_name_in_module.c_str(),
                               m_type_object.get())) {
            error::check_error();
            MGE_THROW(python::error) << "Cannot add type to module";
        }
    }

    void python_type::define_callable_class() {}

    void python_type::on_interpreter_loss()
    {
        m_type_object.release();
        for (auto& [name, attr] : m_attributes) {
            attr.release();
        }
    }

    void python_type::on_interpreter_restore()
    {
        initialize();
        define_in_interpreter();
    }

} // namespace mge::python