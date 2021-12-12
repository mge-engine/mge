// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "python_bind_helper.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/module.hpp"
#include "mge/script/module_details.hpp"
#include "python_context.hpp"

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {
    python_bind_helper::python_bind_helper(python_context& context)
        : m_context(context)
    {}

    python_bind_helper::~python_bind_helper() {}

    void python_bind_helper::bind(const mge::script::module& m) { visit(m); }

    void python_bind_helper::begin(const mge::script::module_details& m)
    {
        mge::script::module_details_ref m_ref =
            const_cast<mge::script::module_details&>(m).shared_from_this();
        MGE_DEBUG_TRACE(PYTHON) << "Binding module '" << m_ref->name() << "'";
        auto mod = mge::script::module(m_ref);
        auto pm = std::make_shared<python_module>(mod);
        m_context.add_module(pm);

        if (!m_module_stack.empty()) {
            m_module_stack.top()->add_module(pm);
        }
        m_module_stack.push(pm);
    }

    void python_bind_helper::end(const mge::script::module_details& m)
    {
        m_module_stack.pop();
    }

    void python_bind_helper::begin(const mge::script::type_details& t)
    {
        if (t.type_class().is_pod() || t.type_class().is_void) {
            mge::script::type_details_ref t_ref =
                const_cast<mge::script::type_details&>(t).shared_from_this();
            m_pod_types[t.type_index()] = t_ref;
        } else if (t.type_class().is_enum) {
            begin_enum(t);
            return;
        }
    }

    void python_bind_helper::enum_value(const std::string&              name,
                                        const mge::script::any_integer& value)
    {
        return;
    }

    void python_bind_helper::end(const mge::script::type_details& t)
    {
        if (t.type_class().is_enum) {
            end_enum(t);
        }
    }

    void python_bind_helper::visit(const mge::script::function_details& v) {}
    void python_bind_helper::visit(const mge::script::variable_details& v) {}

    void python_bind_helper::begin_enum(const mge::script::type_details& t)
    {
        m_current_type = std::make_shared<python_type>(t.name(), 0);
    }

    void python_bind_helper::end_enum(const mge::script::type_details& t)
    {
        MGE_DEBUG_TRACE(PYTHON) << "Add enum type '" << t.name() << "'";
        PyObject* type = m_current_type->materialize_type();
        try {
            auto module = m_module_stack.top();
            module->add_object(t.name().c_str(), type);
        } catch (...) {
            Py_XDECREF(type);
            throw;
        }
    }

} // namespace mge::python