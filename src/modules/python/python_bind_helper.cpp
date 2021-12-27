// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "python_bind_helper.hpp"
#include "mge/core/checked_cast.hpp"
#include "mge/core/overloaded.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/function_details.hpp"
#include "mge/script/module.hpp"
#include "mge/script/module_details.hpp"
#include "python_call_context.hpp"
#include "python_context.hpp"
#include "python_error.hpp"

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {

    static inline PyObject* from_integer(const mge::script::any_integer& i)
    {
        return std::visit(
            overloaded{
                [](const auto& i) { return (PyObject*)(nullptr); },
                [](const char& c) {
                    int i = c;
                    return Py_BuildValue("i", i);
                },
                [](const signed char& c) {
                    int i = c;
                    return Py_BuildValue("i", i);
                },
                [](const unsigned char& uc) { return Py_BuildValue("b", uc); },
                [](const short& s) { return Py_BuildValue("h", s); },
                [](const unsigned short& s) { return Py_BuildValue("H", s); },
                [](const int& i) { return Py_BuildValue("i", i); },
                [](const unsigned int& i) { return Py_BuildValue("I", i); },
                [](const int64_t& i) { return Py_BuildValue("L", i); },
                [](const uint64_t& i) { return Py_BuildValue("K", i); },
            },
            i);
    }

    python_bind_helper::python_bind_helper(python_context& context)
        : m_context(context)
    {
        bind_basic_modules();
    }

    void python_bind_helper::bind_basic_modules()
    {
        auto root_mod = mge::script::module();
        auto python_root_mod = m_context.get_module(root_mod.full_name());
        if (!python_root_mod) {
            MGE_DEBUG_TRACE(PYTHON) << "Creating __main__ module";
            python_root_mod = std::make_shared<python_module>(root_mod);
            m_context.add_module(python_root_mod);
            auto details = root_mod.details();
            m_context.mark_visited(details.get());
        }

        auto mge_mod = mge::script::module("mge");
        auto python_mge_mod = m_context.get_module(mge_mod.full_name());
        if (!python_mge_mod) {
            MGE_DEBUG_TRACE(PYTHON) << "Creating mge module";
            python_mge_mod = std::make_shared<python_module>(mge_mod);
            m_context.add_module(python_mge_mod);
            python_root_mod->add_module(python_mge_mod);
            auto details = mge_mod.details();
            m_context.mark_visited(details.get());
            define_function_type(python_mge_mod);
        }
    }

    python_bind_helper::~python_bind_helper() {}

    void python_bind_helper::bind(const mge::script::module& m) { visit(m); }

    void python_bind_helper::begin(const mge::script::module_details& m)
    {
        MGE_DEBUG_TRACE(PYTHON) << "Binding module '" << m.name() << "'";
        auto pm = m_context.get_module(m.full_name());
        if (!pm) {
            mge::script::module_details_ref m_ref =
                const_cast<mge::script::module_details&>(m).shared_from_this();
            auto mod = mge::script::module(m_ref);
            pm = std::make_shared<python_module>(mod);
            m_context.add_module(pm);

            if (!m_module_stack.empty()) {
                m_module_stack.top()->add_module(pm);
            }
            m_context.mark_visited(&m);
            m_module_stack.push(pm);
        } else {
            m_module_stack.push(pm);
        }
    }

    void python_bind_helper::end(const mge::script::module_details& m)
    {
        m_module_stack.pop();
    }

    void python_bind_helper::begin(const mge::script::type_details& t)
    {
        if (m_context.visited(&t)) {
            return;
        }

        if (t.type_class().is_pod() || t.type_class().is_void) {
            mge::script::type_details_ref t_ref =
                const_cast<mge::script::type_details&>(t).shared_from_this();
            m_pod_types[t.type_index()] = t_ref;
        } else if (t.type_class().is_enum) {
            begin_enum(t);
            return;
        } else if (t.type_class().is_class) {
            begin_class(t);
            return;
        }
    }

    void python_bind_helper::enum_value(const std::string&              name,
                                        const mge::script::any_integer& value)
    {
        PyObject* value_object = from_integer(value);
        if (!value_object) {
            MGE_THROW(mge::illegal_argument)
                << "Cannot handle type of enum value for '" << name << "'";
        }
        try {
            m_current_type->set_attribute(name.c_str(), value_object);
            return;
        } catch (...) {
            Py_XDECREF(value_object);
            throw;
        }
    }

    void python_bind_helper::end(const mge::script::type_details& t)
    {
        if (m_context.visited(&t)) {
            return;
        }

        if (t.type_class().is_enum) {
            end_enum(t);
        } else if (t.type_class().is_class) {
            end_class(t);
        }

        m_context.mark_visited(&t);
    }

    void python_bind_helper::visit(const mge::script::variable_details& v)
    {
        if (m_context.visited(&v)) {
            return;
        }
    }

    void python_bind_helper::begin_enum(const mge::script::type_details& t)
    {
        m_current_type = std::make_shared<python_type>(m_context, t);
    }

    void python_bind_helper::end_enum(const mge::script::type_details& t)
    {
        MGE_DEBUG_TRACE(PYTHON) << "Add enum type '" << t.name() << "'";
        PyObject* type = m_current_type->materialize_type();
        try {
            auto module = m_module_stack.top();
            module->add_object(t.name().c_str(), type);
            m_context.add_type(m_current_type);
            m_current_type.reset();
        } catch (...) {
            Py_XDECREF(type);
            throw;
        }
    }

    void python_bind_helper::begin_class(const mge::script::type_details& t)
    {
        m_current_complex_type =
            std::make_shared<python_complex_type>(m_context, t);
    }

    void python_bind_helper::end_class(const mge::script::type_details& t)
    {
        MGE_DEBUG_TRACE(PYTHON) << "Add object type '" << t.name() << "'";
        PyObject* type = m_current_complex_type->materialize_type();
        try {
            auto module = m_module_stack.top();
            module->add_object(t.name().c_str(), type);
            if (!t.alias().empty()) {
                module->add_object(t.alias().c_str(), type);
            }
            m_context.add_type(m_current_complex_type);
            m_current_complex_type.reset();
        } catch (...) {
            Py_XDECREF(type);
            throw;
        }
    }

    void python_bind_helper::field(const mge::script::field_details& field)
    {
        m_current_complex_type->add_field(field);
    }

    void
    python_bind_helper::constructor(const mge::script::signature&        s,
                                    const mge::script::context_function& cf)
    {
        m_current_complex_type->add_constructor(s, cf);
    }

    struct script_function
    {
        // clang-format off
        PyObject_HEAD
        mge::script::function_details* function;
        python_context *context;
        // clang-format on

        static int init(PyObject* self_object, PyObject* args, PyObject* kwds)
        {
            script_function* self =
                reinterpret_cast<script_function*>(self_object);
            self->function = nullptr;
            return 0;
        }

        static PyObject*
        call(PyObject* callable, PyObject* args, PyObject* kwargs)
        {
            script_function* f = reinterpret_cast<script_function*>(callable);

            PyObject* result = nullptr;

            if (f->function) {
                python_call_context ctx(*f->context, args);
                f->function->invoke()(ctx);
                result = ctx.result();
                Py_INCREF(result);
            } else {
                result = Py_None;
                Py_INCREF(result);
            }

            return result;
        }
    };

    static PyType_Slot s_script_function_slots[] = {
        {Py_tp_init, script_function::init},
        {Py_tp_call, script_function::call},
        {}};

    void python_bind_helper::define_function_type(python_module_ref& mge_module)
    {
        MGE_DEBUG_TRACE(PYTHON) << "Create _script_function helper class";
        PyType_Spec spec = {};
        spec.name = "mge._script_function";
        spec.basicsize = checked_cast<int>(sizeof(script_function));
        spec.flags = Py_TPFLAGS_DEFAULT;
        spec.slots = s_script_function_slots;
        PyObject* function_type = PyType_FromSpec(&spec);
        mge_module->add_object("_script_function", function_type);
        m_context.set_function_type(function_type); // will use the reference
    }

    void python_bind_helper::visit(const mge::script::function_details& f)
    {
        if (m_context.visited(&f)) {
            return;
        }
        MGE_DEBUG_TRACE(PYTHON) << "Add function '" << f.name() << "'";
        PyObject* new_function_obj =
            PyObject_CallNoArgs(m_context.function_type());
        error::check_error();

        script_function* new_function =
            reinterpret_cast<script_function*>(new_function_obj);
        new_function->function = const_cast<mge::script::function_details*>(&f);
        new_function->context = &m_context;
        m_module_stack.top()->add_object(f.name().c_str(), new_function_obj);
        Py_XDECREF(new_function_obj);
    }

} // namespace mge::python