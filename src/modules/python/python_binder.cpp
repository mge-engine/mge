// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "python_binder.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/module.hpp"
#include "mge/script/type_details.hpp"
#include "python_context.hpp"
#include "python_type.hpp"

#include <stack>

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {

    class module_binder : public mge::script::visitor
    {
    public:
        using mge::script::visitor::start;

        module_binder(python_binder& binder)
            : m_binder(binder)
        {}

        virtual void start(const mge::script::module_details_ref& m) override
        {
            mge::script::module mod(m);
            m_binder.context().get_or_add_module(mod);
        }

        python_binder& m_binder;
    };

    class type_binder : public mge::script::visitor
    {
    public:
        type_binder(python_binder& binder)
            : m_binder(binder)
        {}

        void start(const mge::script::module_details_ref& m) override
        {
            mge::script::module mod(m);

            auto pymod = m_binder.context().get_module(mod);
            m_py_modules.push(pymod);
        }

        void finish(const mge::script::module_details_ref& m) override
        {
            m_py_modules.pop();
        }

        void start(const mge::script::type_details_ref& t) override
        {
            if (!t->traits().is_pod()) {
                m_current_type =
                    std::make_shared<python_type>(m_binder.context(), t);
            } else {
                m_current_type.reset();
            }
        }

        void finish(const mge::script::type_details_ref& m) override
        {
            if (m_current_type) {
                m_py_modules.top()->add_type(m_current_type);
                m_current_type.reset();
            }
        }

        void enum_value(const std::string& name, int64_t value)
        {
            m_current_type->add_enum_value(name, value);
        }

        python_binder&                m_binder;
        std::stack<python_module_ref> m_py_modules;
        python_type_ref               m_current_type;
    };

    python_binder::python_binder(python_context& context)
        : m_context(context)
    {}

    void python_binder::bind(const mge::script::module& m)
    {
        module_binder mb(*this);
        m.apply(mb);
        type_binder tb(*this);
        m.apply(tb);
    }

} // namespace mge::python