// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.

#include "python_type.hpp"
#include "python_call_context.hpp"
#include "python_context.hpp"
#include "python_error.hpp"
#include "python_instance.hpp"
#include "python_invocation_context.hpp"
#include "python_module.hpp"

#include "mge/core/trace.hpp"
#include "mge/reflection/module_details.hpp"
#include "mge/reflection/type_details.hpp"

#include <typeindex>
#include <unordered_map>

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {

    namespace {

        // Registry mapping type_details* -> PyObject* (borrowed, non-owning)
        std::unordered_map<const mge::reflection::type_details*, PyObject*>
            s_type_registry;

        bool is_python_compatible(PyObject*                               arg,
                                  const mge::reflection::type_identifier& tid)
        {
            auto ti = tid.type_index();

            if (ti == std::type_index(typeid(bool))) {
                return PyBool_Check(arg) != 0;
            }
            if (ti == std::type_index(typeid(int8_t)) ||
                ti == std::type_index(typeid(uint8_t)) ||
                ti == std::type_index(typeid(int16_t)) ||
                ti == std::type_index(typeid(uint16_t)) ||
                ti == std::type_index(typeid(int32_t)) ||
                ti == std::type_index(typeid(uint32_t)) ||
                ti == std::type_index(typeid(int64_t)) ||
                ti == std::type_index(typeid(uint64_t)) ||
                ti == std::type_index(typeid(long)) ||
                ti == std::type_index(typeid(unsigned long))) {
                return PyLong_Check(arg) != 0;
            }
            if (ti == std::type_index(typeid(float)) ||
                ti == std::type_index(typeid(double)) ||
                ti == std::type_index(typeid(long double))) {
                return PyFloat_Check(arg) != 0 || PyLong_Check(arg) != 0;
            }
            if (ti == std::type_index(typeid(std::string)) ||
                ti == std::type_index(typeid(std::string_view))) {
                return PyUnicode_Check(arg) != 0;
            }

            const auto& type_ref = mge::reflection::type_details::get(tid);
            if (type_ref && type_ref->is_enum) {
                return PyLong_Check(arg) != 0;
            }
            if (type_ref && type_ref->is_primitive_vector) {
                return PyList_Check(arg) != 0 || PyTuple_Check(arg) != 0;
            }
            if (type_ref && (type_ref->is_class || type_ref->is_pointer)) {
                if (arg == Py_None) {
                    return true;
                }
                PyObject* capsule = PyObject_GetAttrString(
                    reinterpret_cast<PyObject*>(Py_TYPE(arg)),
                    "__mge_type_details__");
                if (capsule) {
                    PyErr_Clear();
                    bool ok = PyCapsule_CheckExact(capsule) != 0;
                    Py_DECREF(capsule);
                    return ok;
                }
                PyErr_Clear();
                return false;
            }
            return false;
        }

        void resolve_return_type(
            const mge::reflection::type_identifier& return_type_id,
            const mge::reflection::type_details*&   pointer_element_type,
            const mge::reflection::type_details*&   shared_ptr_element_type)
        {
            pointer_element_type = nullptr;
            shared_ptr_element_type = nullptr;

            const auto& return_type_details =
                mge::reflection::type_details::get(return_type_id);
            if (return_type_details && return_type_details->is_pointer) {
                const auto& ptr_details = std::get<
                    mge::reflection::type_details::pointer_specific_details>(
                    return_type_details->specific_details);
                if (ptr_details.element_type &&
                    ptr_details.element_type->is_class) {
                    pointer_element_type = ptr_details.element_type.get();
                }
            } else if (return_type_details && return_type_details->is_class) {
                const auto& ret_class = std::get<
                    mge::reflection::type_details::class_specific_details>(
                    return_type_details->specific_details);
                if (ret_class.is_shared_ptr &&
                    ret_class.shared_ptr_element_type) {
                    shared_ptr_element_type =
                        ret_class.shared_ptr_element_type.get();
                }
            }
            if (!pointer_element_type && !shared_ptr_element_type &&
                return_type_id.is_reference()) {
                auto        base_id = return_type_id.base_type_identifier();
                const auto& base_details =
                    mge::reflection::type_details::get(base_id);
                if (base_details && base_details->is_class) {
                    pointer_element_type = base_details.get();
                }
            }
        }

        class instance_method_closure
            : public mge::closure<PyObject*, PyObject*, PyObject*>
        {
        public:
            instance_method_closure(const mge::reflection::type_details* td,
                                    size_t                               method_index)
                : m_type_details(td)
                , m_method_index(method_index)
            {}

            PyObject* execute(PyObject* self, PyObject* args) override
            {
                auto* h =
                    reinterpret_cast<python_instance_header*>(self);
                if (!h->type) {
                    PyErr_SetString(PyExc_RuntimeError, "invalid instance");
                    return nullptr;
                }

                const auto& class_details = std::get<
                    mge::reflection::type_details::class_specific_details>(
                    h->type->specific_details);
                const auto& [name, sig, invoke_fn, is_const, is_noexcept] =
                    class_details.methods[m_method_index];

                Py_ssize_t  nargs = PyTuple_Size(args);
                const auto& params = sig.parameter_types();
                if (static_cast<Py_ssize_t>(params.size()) != nargs) {
                    PyErr_Format(PyExc_TypeError,
                                 "method '%s' expects %d args, got %d",
                                 std::string(name).c_str(),
                                 static_cast<int>(params.size()),
                                 static_cast<int>(nargs));
                    return nullptr;
                }

                for (Py_ssize_t i = 0; i < nargs; ++i) {
                    if (!is_python_compatible(PyTuple_GET_ITEM(args, i),
                                              params[static_cast<size_t>(i)])) {
                        PyErr_Format(PyExc_TypeError,
                                     "incompatible argument %d for method '%s'",
                                     static_cast<int>(i) + 1,
                                     std::string(name).c_str());
                        return nullptr;
                    }
                }

                const mge::reflection::type_details* pointer_element_type =
                    nullptr;
                const mge::reflection::type_details* shared_ptr_element_type =
                    nullptr;
                resolve_return_type(sig.return_type(),
                                    pointer_element_type,
                                    shared_ptr_element_type);

                void*                obj_ptr = h->object.get();
                python_call_context  ctx(args, 0, obj_ptr);
                if (pointer_element_type)
                    ctx.set_pointer_result_type(pointer_element_type);
                if (shared_ptr_element_type)
                    ctx.set_shared_ptr_result_type(shared_ptr_element_type);
                invoke_fn(ctx);
                return ctx.result();
            }

        private:
            const mge::reflection::type_details* m_type_details;
            size_t                               m_method_index;
        };

        class static_method_closure
            : public mge::closure<PyObject*, PyObject*, PyObject*>
        {
        public:
            static_method_closure(const mge::reflection::type_details* td,
                                  size_t                               method_index)
                : m_type_details(td)
                , m_method_index(method_index)
            {}

            PyObject* execute(PyObject* /*self*/, PyObject* args) override
            {
                const auto& class_details = std::get<
                    mge::reflection::type_details::class_specific_details>(
                    m_type_details->specific_details);
                const auto& [name, sig, invoke_fn] =
                    class_details.static_methods[m_method_index];

                Py_ssize_t  nargs = PyTuple_Size(args);
                const auto& params = sig.parameter_types();
                if (static_cast<Py_ssize_t>(params.size()) != nargs) {
                    PyErr_Format(PyExc_TypeError,
                                 "static method '%s' expects %d args, got %d",
                                 std::string(name).c_str(),
                                 static_cast<int>(params.size()),
                                 static_cast<int>(nargs));
                    return nullptr;
                }

                for (Py_ssize_t i = 0; i < nargs; ++i) {
                    if (!is_python_compatible(PyTuple_GET_ITEM(args, i),
                                              params[static_cast<size_t>(i)])) {
                        PyErr_Format(
                            PyExc_TypeError,
                            "incompatible argument %d for static method '%s'",
                            static_cast<int>(i) + 1,
                            std::string(name).c_str());
                        return nullptr;
                    }
                }

                const mge::reflection::type_details* pointer_element_type =
                    nullptr;
                const mge::reflection::type_details* shared_ptr_element_type =
                    nullptr;
                resolve_return_type(sig.return_type(),
                                    pointer_element_type,
                                    shared_ptr_element_type);

                python_call_context ctx(args, 0, nullptr);
                if (pointer_element_type)
                    ctx.set_pointer_result_type(pointer_element_type);
                if (shared_ptr_element_type)
                    ctx.set_shared_ptr_result_type(shared_ptr_element_type);
                invoke_fn(ctx);
                return ctx.result();
            }

        private:
            const mge::reflection::type_details* m_type_details;
            size_t                               m_method_index;
        };

    } // anonymous namespace

    python_type::python_type(python_context&                 context,
                             const reflection::type_details& details)
        : m_context(context)
        , m_type_details(details)
        , m_name(details.name)
    {
        m_module_name = details.module.lock()->full_name();

        if (details.is_enum) {
            init_enum();
        } else if (details.is_class) {
            init_class();
        }
    }

    python_type::~python_type()
    {
        s_type_registry.erase(&m_type_details);
    }

    PyObject*
    python_type::create_foreign_instance(
        const mge::reflection::type_details* target_type, void* ptr)
    {
        auto it = s_type_registry.find(target_type);
        if (it == s_type_registry.end() || !ptr) {
            Py_INCREF(Py_None);
            return Py_None;
        }

        auto* type_obj = reinterpret_cast<PyTypeObject*>(it->second);
        auto* self = type_obj->tp_alloc(type_obj, 0);
        if (self) {
            auto* h = reinterpret_cast<python_instance_header*>(self);
            new (&h->object) std::shared_ptr<void>(ptr, [](void*) {});
            h->type = target_type;
        }
        return self;
    }

    PyObject*
    python_type::create_shared_instance(
        const mge::reflection::type_details* element_type,
        std::shared_ptr<void>                ptr)
    {
        auto it = s_type_registry.find(element_type);
        if (it == s_type_registry.end() || !ptr) {
            Py_INCREF(Py_None);
            return Py_None;
        }

        auto* type_obj = reinterpret_cast<PyTypeObject*>(it->second);
        auto* self = type_obj->tp_alloc(type_obj, 0);
        if (self) {
            auto* h = reinterpret_cast<python_instance_header*>(self);
            new (&h->object) std::shared_ptr<void>(std::move(ptr));
            h->type = element_type;
        }
        return self;
    }

    PyObject* python_type::tp_new_static(PyTypeObject* type,
                                         PyObject* /*args*/,
                                         PyObject* /*kwds*/)
    {
        PyObject* self = type->tp_alloc(type, 0);
        if (self) {
            auto* h = reinterpret_cast<python_instance_header*>(self);
            new (&h->object) std::shared_ptr<void>();
            h->type = nullptr;

            PyObject* capsule = PyObject_GetAttrString(
                reinterpret_cast<PyObject*>(type), "__mge_type_details__");
            if (capsule && PyCapsule_CheckExact(capsule)) {
                h->type =
                    static_cast<const mge::reflection::type_details*>(
                        PyCapsule_GetPointer(capsule, "__mge_type_details__"));
            }
            Py_XDECREF(capsule);
            PyErr_Clear();
        }
        return self;
    }

    int python_type::tp_init_static(PyObject* self,
                                    PyObject* args,
                                    PyObject* kwds)
    {
        if (kwds && PyDict_Size(kwds) > 0) {
            PyErr_SetString(PyExc_TypeError, "keyword arguments not supported");
            return -1;
        }

        auto* h = reinterpret_cast<python_instance_header*>(self);
        if (!h->type || !h->type->is_class) {
            PyErr_SetString(PyExc_RuntimeError, "invalid class instance");
            return -1;
        }

        const auto& class_details = std::get<
            mge::reflection::type_details::class_specific_details>(
            h->type->specific_details);

        const mge::reflection::type_details* proxy_type =
            class_details.proxy_type.get();
        const auto& ctor_source =
            proxy_type
                ? std::get<mge::reflection::type_details::class_specific_details>(
                      proxy_type->specific_details)
                : class_details;

        Py_ssize_t nargs = PyTuple_Size(args);

        const mge::reflection::invoke_function* matched_ctor = nullptr;
        for (const auto& [sig, invoke_fn] : ctor_source.constructors) {
            const auto& params = sig.parameter_types();
            if (static_cast<Py_ssize_t>(params.size()) != nargs) {
                continue;
            }
            bool compatible = true;
            for (Py_ssize_t i = 0; i < nargs; ++i) {
                if (!is_python_compatible(PyTuple_GET_ITEM(args, i),
                                          params[static_cast<size_t>(i)])) {
                    compatible = false;
                    break;
                }
            }
            if (compatible) {
                matched_ctor = &invoke_fn;
                break;
            }
        }

        if (!matched_ctor) {
            PyErr_Format(PyExc_TypeError,
                         "no matching constructor for '%s' with %d args",
                         std::string(h->type->name).c_str(),
                         static_cast<int>(nargs));
            return -1;
        }

        size_t obj_size = proxy_type ? proxy_type->size : h->type->size;
        void*  raw = ::operator new(obj_size);

        python_call_context ctx(args, 0, raw);
        (*matched_ctor)(ctx);

        if (ctx.had_exception()) {
            ::operator delete(raw);
            return -1;
        }

        auto raw_dtor = ctor_source.raw_destructor;

        if (proxy_type && class_details.set_context) {
            auto* inv_ctx = new python_invocation_context(self);
            class_details.set_context(raw, inv_ctx);
            auto set_ctx = class_details.set_context;
            h->object = std::shared_ptr<void>(
                raw, [raw_dtor, inv_ctx, set_ctx](void* p) {
                    set_ctx(p, nullptr);
                    if (raw_dtor) {
                        raw_dtor(p);
                    }
                    ::operator delete(p);
                    delete inv_ctx;
                });
        } else {
            h->object = std::shared_ptr<void>(raw, [raw_dtor](void* p) {
                if (raw_dtor) {
                    raw_dtor(p);
                }
                ::operator delete(p);
            });
        }

        return 0;
    }

    void python_type::tp_finalize_static(PyObject* self)
    {
        auto* h = reinterpret_cast<python_instance_header*>(self);
        h->object.~shared_ptr<void>();
    }

    PyObject* python_type::field_getter_static(PyObject* self, void* closure)
    {
        auto* desc = static_cast<field_descriptor*>(closure);
        auto* h = reinterpret_cast<python_instance_header*>(self);
        if (!h->type || !h->object) {
            PyErr_SetString(PyExc_RuntimeError, "invalid instance");
            return nullptr;
        }

        const auto& class_details = std::get<
            mge::reflection::type_details::class_specific_details>(
            desc->type->specific_details);
        const auto& [name, type_id, getter_fn, setter_fn] =
            class_details.fields[desc->field_index];

        void*               obj_ptr = h->object.get();
        python_call_context ctx(nullptr, 0, obj_ptr);
        getter_fn(ctx);
        return ctx.result();
    }

    int python_type::field_setter_static(PyObject* self,
                                         PyObject* value,
                                         void*     closure)
    {
        auto* desc = static_cast<field_descriptor*>(closure);
        auto* h = reinterpret_cast<python_instance_header*>(self);
        if (!h->type || !h->object) {
            PyErr_SetString(PyExc_RuntimeError, "invalid instance");
            return -1;
        }

        const auto& class_details = std::get<
            mge::reflection::type_details::class_specific_details>(
            desc->type->specific_details);
        const auto& [name, type_id, getter_fn, setter_fn] =
            class_details.fields[desc->field_index];

        if (!setter_fn) {
            PyErr_Format(PyExc_AttributeError,
                         "field '%s' is read-only",
                         std::string(name).c_str());
            return -1;
        }

        PyObject* args = PyTuple_Pack(1, value);
        if (!args) {
            return -1;
        }

        void*               obj_ptr = h->object.get();
        python_call_context ctx(args, 0, obj_ptr);
        setter_fn(ctx);
        Py_DECREF(args);

        if (ctx.had_exception()) {
            return -1;
        }
        PyObject* r = ctx.result();
        Py_XDECREF(r);
        return 0;
    }

    void python_type::init_enum()
    {
        MGE_DEBUG_TRACE(PYTHON,
                        "Creating python type for enum: {}",
                        m_type_details.name);
        if (m_type_slots.empty() || m_type_slots.rbegin()->slot != 0) {
            m_type_slots.push_back({0, nullptr});
        }

        m_spec = {.name = m_name.c_str(),
                  .basicsize = sizeof(PyLongObject),
                  .flags = Py_TPFLAGS_BASETYPE | Py_TPFLAGS_LONG_SUBCLASS |
                           Py_TPFLAGS_HEAPTYPE,
                  .slots = m_type_slots.data()};
        for (const auto& [val_name, val] :
             m_type_details.enum_specific().values) {
            if (std::holds_alternative<int64_t>(val)) {
                m_attributes[std::string(val_name)] =
                    pyobject_ref(PyLong_FromLongLong(std::get<int64_t>(val)));
            } else if (std::holds_alternative<uint64_t>(val)) {
                m_attributes[std::string(val_name)] = pyobject_ref(
                    PyLong_FromUnsignedLongLong(std::get<uint64_t>(val)));
            }
        }

        python_module_ref module = m_context.module(m_module_name);
        PyObject*         base = reinterpret_cast<PyObject*>(&PyLong_Type);

        m_type_object = PyType_FromModuleAndSpec(
            m_context.module(m_module_name)->py_module().get(),
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

        if (PyModule_AddObject(module->py_module().get(),
                               m_name.c_str(),
                               m_type_object.get())) {
            error::check_error();
            MGE_THROW(python::error) << "Cannot add type '" << m_name
                                     << "' to module '" << m_module_name << "'";
        }
    }

    void python_type::init_class()
    {
        MGE_DEBUG_TRACE(PYTHON,
                        "Creating python type for class: {}",
                        m_type_details.name);

        const auto& class_details = std::get<
            mge::reflection::type_details::class_specific_details>(
            m_type_details.specific_details);

        // Reserve to prevent reallocation that would invalidate c_str() pointers
        m_string_storage.reserve(class_details.methods.size() +
                                 class_details.static_methods.size() +
                                 class_details.fields.size());
        m_field_descriptors.reserve(class_details.fields.size());
        m_type_methods.reserve(class_details.methods.size() +
                               class_details.static_methods.size() + 1);
        m_type_fields.reserve(class_details.fields.size() + 1);

        // --- type lifecycle slots ---
        m_type_slots.push_back(
            {Py_tp_new, reinterpret_cast<void*>(&python_type::tp_new_static)});
        m_type_slots.push_back(
            {Py_tp_init,
             reinterpret_cast<void*>(&python_type::tp_init_static)});
        m_type_slots.push_back(
            {Py_tp_finalize,
             reinterpret_cast<void*>(&python_type::tp_finalize_static)});

        // --- instance methods ---
        for (size_t i = 0; i < class_details.methods.size(); ++i) {
            const auto& [name, sig, invoke_fn, is_const, is_noexcept] =
                class_details.methods[i];

            auto c = std::make_unique<instance_method_closure>(&m_type_details, i);
            PyCFunction fn_ptr = c->function();
            m_closures.push_back(std::move(c));

            m_string_storage.emplace_back(name);
            m_type_methods.push_back(
                {m_string_storage.back().c_str(), fn_ptr, METH_VARARGS, nullptr});
        }

        // --- static methods ---
        for (size_t i = 0; i < class_details.static_methods.size(); ++i) {
            const auto& [name, sig, invoke_fn] = class_details.static_methods[i];

            auto c =
                std::make_unique<static_method_closure>(&m_type_details, i);
            PyCFunction fn_ptr = c->function();
            m_closures.push_back(std::move(c));

            m_string_storage.emplace_back(name);
            m_type_methods.push_back({m_string_storage.back().c_str(),
                                      fn_ptr,
                                      METH_VARARGS | METH_STATIC,
                                      nullptr});
        }

        m_type_methods.push_back({nullptr, nullptr, 0, nullptr});
        if (m_type_methods.size() > 1) {
            m_type_slots.push_back(
                {Py_tp_methods,
                 reinterpret_cast<void*>(m_type_methods.data())});
        }

        // --- fields as getset descriptors ---
        for (size_t i = 0; i < class_details.fields.size(); ++i) {
            const auto& [name, type_id, getter_fn, setter_fn] =
                class_details.fields[i];

            m_field_descriptors.push_back({&m_type_details, i});
            m_string_storage.emplace_back(name);

            PyGetSetDef gsd;
            gsd.name    = m_string_storage.back().c_str();
            gsd.get     = &python_type::field_getter_static;
            gsd.set     = setter_fn ? &python_type::field_setter_static : nullptr;
            gsd.doc     = nullptr;
            gsd.closure = &m_field_descriptors.back();
            m_type_fields.push_back(gsd);
        }

        m_type_fields.push_back({nullptr, nullptr, nullptr, nullptr, nullptr});
        if (m_type_fields.size() > 1) {
            m_type_slots.push_back(
                {Py_tp_getset,
                 reinterpret_cast<void*>(m_type_fields.data())});
        }

        m_type_slots.push_back({0, nullptr});

        // strip namespace prefix for the Python-visible name
        std::string_view short_name = m_type_details.alias.empty()
                                          ? m_type_details.name
                                          : m_type_details.alias;
        auto pos = short_name.rfind("::");
        if (pos != std::string_view::npos) {
            short_name = short_name.substr(pos + 2);
        }

        m_spec = {.name      = m_name.c_str(),
                  .basicsize = static_cast<int>(sizeof(python_instance_header)),
                  .flags     = Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HEAPTYPE,
                  .slots     = m_type_slots.data()};

        python_module_ref module = m_context.module(m_module_name);
        m_type_object = PyType_FromModuleAndSpec(module->py_module().get(),
                                                 &m_spec,
                                                 nullptr);
        if (!m_type_object) {
            error::check_error();
        }

        // Store type_details pointer as a capsule attribute so tp_new_static
        // can retrieve it without needing a per-type closure.
        PyObject* capsule =
            PyCapsule_New(const_cast<mge::reflection::type_details*>(&m_type_details),
                          "__mge_type_details__",
                          nullptr);
        if (capsule) {
            PyObject_SetAttrString(m_type_object.get(),
                                   "__mge_type_details__",
                                   capsule);
            Py_DECREF(capsule);
        }

        s_type_registry[&m_type_details] = m_type_object.get();

        if (PyModule_AddObject(module->py_module().get(),
                               std::string(short_name).c_str(),
                               m_type_object.get())) {
            error::check_error();
            MGE_THROW(python::error)
                << "Cannot add type '" << m_name << "' to module '"
                << m_module_name << "'";
        }
    }

} // namespace mge::python
