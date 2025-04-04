// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "python_type.hpp"
#include "gil_lock.hpp"
#include "python_call_context.hpp"
#include "python_context.hpp"
#include "python_error.hpp"
#include "python_module.hpp"
#include "type_matches.hpp"

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
        if (m_name_in_module.find("::") != std::string::npos ||
            m_name_in_module.find("<") != std::string::npos) {
            m_name_in_module = type->generic_name();
        }

        if (m_type->exposed_directly()) {
            m_module_name = m_type->module().lock()->full_name();
        } else {
            m_module_name = "__mge__";
        }
        m_name = m_module_name + "." + m_name_in_module;
        MGE_DEBUG_TRACE(PYTHON)
            << "Creating type " << type->name() << " as " << m_name;
        initialize();
    }

    void python_type::initialize()
    {
        if (m_type->is_enum()) {
            init_enum();
        } else if (m_type->is_class()) {
            init_class();
        } else {
            MGE_DEBUG_TRACE(PYTHON) << "Unsupported type: " << m_type->name();
        }
    }

    void python_type::init_enum()
    {
        gil_lock guard;
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
                gil_lock guard;
                object*  self;
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
                    python_call_context ctx(nullptr, &obj->shared_ptr_address);
                    m_type->class_specific().destroy_shared(ctx);
                    // and set shared_ptr_address to nullptr
                    // to avoid double deletion
                    obj->shared_ptr_address = nullptr;
                }
                {
                    gil_lock guard;
                    Py_TYPE(self)->tp_free(self);
                }
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

        init_fields();
        init_methods();

        m_type_slots.emplace_back(Py_tp_new, m_tp_new_closure->function());
        if (!m_type->class_specific().constructors.empty()) {
            m_type_slots.emplace_back(Py_tp_dealloc,
                                      m_tp_dealloc_closure->function());
            m_type_slots.emplace_back(Py_tp_init,
                                      m_tp_init_closure->function());
        }
        if (!m_type_fields.empty()) {
            m_type_fields.push_back(
                {nullptr, nullptr, nullptr, nullptr, nullptr});
            m_type_slots.emplace_back(Py_tp_getset, m_type_fields.data());
        }
        if (!m_type_methods.empty()) {
            m_type_methods.push_back({nullptr, nullptr, 0, nullptr});
            m_type_slots.emplace_back(Py_tp_methods, m_type_methods.data());
        }
    }

    void python_type::add_field(const std::string&                  name,
                                const mge::script::invoke_function& getter,
                                const mge::script::invoke_function& setter)
    {
        struct get_closure : tp_get_closure
        {
            get_closure(
                const mge::script::invoke_function& getter,
                const mge::script::type_data::
                    extract_this_from_shared_ptr_address& this_from_shared_ptr)
                : m_getter(getter)
                , m_this_from_shared_ptr(this_from_shared_ptr)
            {}

            PyObject* execute(PyObject* self, void* closure)
            {
                gil_lock guard;
                object*  obj = reinterpret_cast<object*>(self);
                void*    this_ptr =
                    m_this_from_shared_ptr(obj->shared_ptr_address);
                python_call_context ctx(this_ptr, obj->shared_ptr_address);
                m_getter(ctx);
                if (ctx.has_exception()) {
                    return nullptr;
                }
                return ctx.result();
            }
            const mge::script::invoke_function& m_getter;
            const mge::script::type_data::extract_this_from_shared_ptr_address&
                m_this_from_shared_ptr;
        };

        struct set_closure : tp_set_closure
        {
            set_closure(
                const mge::script::invoke_function& setter,
                const mge::script::type_data::
                    extract_this_from_shared_ptr_address& this_from_shared_ptr)
                : m_setter(setter)
                , m_this_from_shared_ptr(this_from_shared_ptr)
            {}

            int execute(PyObject* self, PyObject* value, void* closure)
            {
                gil_lock guard;
                object*  obj = reinterpret_cast<object*>(self);

                void* this_ptr =
                    m_this_from_shared_ptr(obj->shared_ptr_address);
                python_call_context ctx(this_ptr, obj->shared_ptr_address);
                ctx.set_single_argument(value);
                m_setter(ctx);
                if (ctx.has_exception()) {
                    return -1;
                }
                return 0;
            }
            const mge::script::invoke_function& m_setter;
            const mge::script::type_data::extract_this_from_shared_ptr_address&
                m_this_from_shared_ptr;
        };

        std::shared_ptr<tp_get_closure> getter_closure =
            std::make_shared<get_closure>(
                getter,
                m_type->class_specific().this_from_shared_ptr);
        m_tp_get_closures.push_back(getter_closure);

        std::shared_ptr<tp_set_closure> setter_closure;
        if (setter) {
            setter_closure = std::make_shared<set_closure>(
                setter,
                m_type->class_specific().this_from_shared_ptr);
            m_tp_set_closures.push_back(setter_closure);
        }

        MGE_DEBUG_TRACE(PYTHON) << "Adding field " << name;

        m_type_fields.push_back({name.c_str(),
                                 getter_closure->function(),
                                 setter ? setter_closure->function() : nullptr,
                                 nullptr,
                                 nullptr});
    }

    void python_type::init_fields()
    {
        MGE_DEBUG_TRACE(PYTHON) << "Initializing fields for " << m_name;

        for (const auto& f : m_type->class_specific().fields) {
            add_field(std::get<0>(f), std::get<2>(f), std::get<3>(f));
        }
    }

    void python_type::add_method(
        const std::string&                            name,
        const mge::script::type_data_ref&             return_type,
        const mge::script::type_data::call_signature& signature,
        const mge::script::invoke_function&           method)
    {
        struct method_closure : mge::closure<PyObject*, PyObject*, PyObject*>
        {
            method_closure(
                const mge::script::invoke_function&           method,
                const mge::script::type_data::call_signature& signature,
                const mge::script::type_data_ref&             return_type,
                const mge::script::type_data::
                    extract_this_from_shared_ptr_address& this_from_shared_ptr)
                : m_method(method)
                , m_signature(signature)
                , m_return_type(return_type)
                , m_this_from_shared_ptr(this_from_shared_ptr)
            {}

            PyObject* execute(PyObject* self, PyObject* args)
            {
                gil_lock guard;
                object*  obj = reinterpret_cast<object*>(self);
                void*    this_ptr =
                    m_this_from_shared_ptr(obj->shared_ptr_address);
                python_call_context ctx(this_ptr, obj->shared_ptr_address);
                ctx.set_arguments(args);
                m_method(ctx);
                if (ctx.has_exception()) {
                    return nullptr;
                }
                return ctx.result();
            }

            const mge::script::invoke_function&           m_method;
            const mge::script::type_data::call_signature& m_signature;
            const mge::script::type_data_ref&             m_return_type;
            const mge::script::type_data::extract_this_from_shared_ptr_address&
                m_this_from_shared_ptr;
        };

        std::shared_ptr<method_closure> closure =
            std::make_shared<method_closure>(
                method,
                signature,
                return_type,
                m_type->class_specific().this_from_shared_ptr);

        m_type_methods.push_back(
            {name.c_str(), closure->function(), METH_VARARGS, nullptr});
        m_method_closures.push_back(closure);

        MGE_DEBUG_TRACE(PYTHON) << "Adding method " << name;
    }

    void python_type::init_methods()
    {
        for (const auto& m : m_type->class_specific().methods) {
            add_method(std::get<0>(m),
                       std::get<1>(m),
                       std::get<2>(m),
                       std::get<3>(m));
        }
    }

    int
    python_type::tp_init(PyObject* self, PyObject* args, PyObject* kwargs) const
    {

        gil_lock             gil_guard;
        python_type::object* obj = reinterpret_cast<python_type::object*>(self);

        if (m_type->class_specific().constructors.empty()) {
            PyErr_SetString(PyExc_TypeError, "No constructors available");
            return -1;
        }

        if (kwargs && PyDict_Check(kwargs) && PyDict_Size(kwargs) > 0) {
            PyErr_SetString(PyExc_TypeError, "Keyword arguments not supported");
            return -1;
        }
        if (!PyTuple_Check(args)) {
            PyErr_SetString(PyExc_TypeError, "Arguments must be a tuple");
            return -1;
        }

        size_t constructor_index = select_constructor(args);

        if (constructor_index ==
            m_type->class_specific().make_shared_constructors.size()) {
            PyErr_SetString(PyExc_TypeError, "No matching constructor found");
            return -1;
        }

        // no python c api call from here on in this function
        gil_guard.release();

        python_call_context ctx(nullptr, &obj->shared_ptr_address);
        ctx.set_arguments(args);

        m_type->class_specific()
            .make_shared_constructors[constructor_index]
            .second(ctx);

        if (ctx.has_exception()) {
            return -1;
        } else {
            return 0;
        }
    }

    static bool better_match(const std::vector<match_type>& current,
                             const std::vector<match_type>& best)
    {
        if (current.empty()) {
            return false;
        }
        if (best.empty()) {
            return true;
        }
        for (size_t i = 0; i < current.size(); ++i) {
            // cast match is smaller than exact match,
            // so new match must improve
            if (current[i] < best[i]) {
                return false;
            }
        }
        return true;
    }

    size_t python_type::select_constructor(PyObject* args) const
    {
        gil_lock guard;
        size_t   arg_count = mge::checked_cast<size_t>(PyTuple_Size(args));
        size_t   constructor_index =
            m_type->class_specific().make_shared_constructors.size();
        std::vector<match_type> best_match;
        for (size_t i = 0;
             i < m_type->class_specific().make_shared_constructors.size();
             ++i) {
            const auto& signature =
                m_type->class_specific().make_shared_constructors[i].first;
            if (signature.size() == arg_count) {
                if (arg_count == 0) {
                    return i;
                }
                std::vector<match_type> current_match;
                for (size_t j = 0; j < arg_count; ++j) {
                    PyObject* arg = PyTuple_GetItem(args, j);
                    if (arg == nullptr) {
                        PyErr_SetString(PyExc_TypeError,
                                        "Cannot get argument from tuple");
                        return m_type->class_specific()
                            .make_shared_constructors.size();
                    }
                    auto m = type_matches(arg, signature[j]);
                    if (m == match_type::NO_MATCH) {
                        current_match.clear();
                        break;
                    } else {
                        current_match.push_back(m);
                    }
                }
                if (better_match(current_match, best_match)) {
                    best_match = std::move(current_match);
                    constructor_index = i;
                }
            }
        }

        return constructor_index;
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
        gil_lock guard;

        python_module_ref module = m_context.module(m_module_name);
        PyObject*         base = reinterpret_cast<PyObject*>(&PyLong_Type);

        m_type_object = PyType_FromModuleAndSpec(
            m_context.module(m_module_name)->pymodule().get(),
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
        MGE_DEBUG_TRACE(PYTHON)
            << "Defining python type " << m_name << " for " << m_type->name();

        gil_lock guard;

        if (m_type_slots.empty() || m_type_slots.rbegin()->slot != 0) {
            m_type_slots.push_back({0, nullptr});
        }

        m_spec = {.name = m_name.c_str(),
                  .basicsize =
                      mge::checked_cast<int>(sizeof(PyObject) + sizeof(void*)),
                  .flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HEAPTYPE,
                  .slots = m_type_slots.data()};

        python_module_ref module = m_context.module(m_module_name);
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