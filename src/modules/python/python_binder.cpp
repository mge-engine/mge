// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "python_binder.hpp"
#include "mge/script/module.hpp"
#include "python_context.hpp"
#include "python_type.hpp"

#include <stack>

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
        using mge::script::visitor::finish;
        using mge::script::visitor::start;

        type_binder(python_binder& binder)
            : m_binder(binder)
        {}

        virtual void start(const mge::script::module_details_ref& m) override
        {
            mge::script::module mod(m);

            auto pymod = m_binder.context().get_module(mod);
            m_py_modules.push(pymod);
        }

        virtual void finish(const mge::script::module_details_ref& m) override
        {
            m_py_modules.pop();
        }

        virtual void start(const mge::script::type_details_ref& m) override {}

        virtual void finish(const mge::script::type_details_ref& m) override
        {
            auto type_ref =
                python_type::make_python_type(m_binder.context(), m);
            if (type_ref) {
                m_py_modules.top()->add_type(type_ref);
            }
        }

        python_binder&                m_binder;
        std::stack<python_module_ref> m_py_modules;
    };

    python_binder::python_binder(python_context& context)
        : m_context(context)
    {}

    python_binder::~python_binder() {}

    void python_binder::bind(const mge::script::module& m)
    {
        module_binder mb(*this);
        m.apply(mb);
    }

} // namespace mge::python