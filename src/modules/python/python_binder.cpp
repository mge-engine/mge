// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "python_binder.hpp"

#include "mge/core/closure.hpp"
#include "mge/core/trace.hpp"

#include "mge/reflection/function_details.hpp"
#include "mge/reflection/module_details.hpp"
#include "mge/reflection/type_details.hpp"
#include "mge/reflection/type_identifier.hpp"

#include "python_call_context.hpp"
#include "python_context.hpp"
#include "python_module.hpp"
#include "python_type.hpp"

#include <typeindex>

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {

    namespace {

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
                if (arg == Py_None)
                    return true;
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

        class function_closure
            : public mge::closure<PyObject*, PyObject*, PyObject*>
        {
        public:
            explicit function_closure(
                const mge::reflection::function_details* details)
                : m_details(details)
            {}

            PyObject* execute(PyObject* /*self*/, PyObject* args) override
            {
                const auto& sig    = m_details->signature();
                const auto& params = sig.parameter_types();
                Py_ssize_t  nargs  = PyTuple_Size(args);

                if (static_cast<Py_ssize_t>(params.size()) != nargs) {
                    PyErr_Format(PyExc_TypeError,
                                 "function '%s' expects %d args, got %d",
                                 m_details->name().c_str(),
                                 static_cast<int>(params.size()),
                                 static_cast<int>(nargs));
                    return nullptr;
                }

                for (Py_ssize_t i = 0; i < nargs; ++i) {
                    if (!is_python_compatible(PyTuple_GET_ITEM(args, i),
                                              params[static_cast<size_t>(i)])) {
                        PyErr_Format(
                            PyExc_TypeError,
                            "incompatible argument %d for function '%s'",
                            static_cast<int>(i) + 1,
                            m_details->name().c_str());
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
                m_details->invoke(ctx);
                return ctx.result();
            }

        private:
            const mge::reflection::function_details* m_details;
        };

    } // anonymous namespace

    python_binder::~python_binder() {}

    void python_binder::before(const mge::reflection::module_details& details)
    {}
    void python_binder::on(const mge::reflection::module_details& details)
    {
        if (details.is_root()) {
            return;
        }
        auto mod = std::make_shared<python_module>(
            m_context,
            std::const_pointer_cast<mge::reflection::module_details>(
                details.shared_from_this()));
        m_context.add_module(mod);
        m_module_stack.push_back(mod);
    }

    void python_binder::after(const mge::reflection::module_details& details)
    {
        if (!details.is_root()) {
            m_module_stack.pop_back();
        }
    }

    void python_binder::before(const mge::reflection::type_details& details) {}

    void python_binder::on(const mge::reflection::type_details& details)
    {
        MGE_DEBUG_TRACE(PYTHON,
                        "Binding type: {}",
                        std::string(details.name).c_str());
        if (details.is_enum) {
            bind_enum(details);
        } else if (details.is_class) {
            bind_class(details);
        }
    }

    void python_binder::after(const mge::reflection::type_details& details) {}

    void python_binder::before(const mge::reflection::function_details& details)
    {}

    void python_binder::on(const mge::reflection::function_details& details)
    {
        if (m_module_stack.empty()) {
            return;
        }
        MGE_DEBUG_TRACE(PYTHON,
                        "Binding function: {}",
                        details.name().c_str());

        auto c = std::make_unique<function_closure>(&details);
        PyCFunction fn_ptr = c->function();
        m_module_stack.back()->add_function(details.name(),
                                            std::move(c),
                                            fn_ptr);
    }

    void python_binder::after(const mge::reflection::function_details& details)
    {}

    void python_binder::bind_enum(const mge::reflection::type_details& details)
    {
        MGE_DEBUG_TRACE(PYTHON, "Binding enum: {}", details.name);
        auto type = std::make_shared<python_type>(m_context, details);
        m_module_stack.back()->add(type);
    }

    void python_binder::bind_class(const mge::reflection::type_details& details)
    {
        MGE_DEBUG_TRACE(PYTHON, "Binding class: {}", details.name);
        auto type = std::make_shared<python_type>(m_context, details);
        m_module_stack.back()->add(type);
    }

} // namespace mge::python
