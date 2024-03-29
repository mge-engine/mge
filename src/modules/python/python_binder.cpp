// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "python_binder.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/module.hpp"
#include "mge/script/type_details.hpp"
#include "python_error.hpp"

#include "boost/boost_algorithm_string.hpp"

#include "python_context.hpp"

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

    class type_creator : public mge::script::visitor
    {
    public:
        type_creator(python_binder& binder)
            : m_binder(binder)
        {}

        using mge::script::visitor::finish;
        using mge::script::visitor::start;

        void start(const mge::script::type_details_ref& t) override
        {
            if (!t->traits().is_pod()) {
                auto pt = std::make_shared<python_type>(m_binder.context(), t);
                m_types.push(pt);
            } else {
                m_types.push(python_type_ref());
            }
        }

        void finish(const mge::script::type_details_ref& m) override
        {
            auto pt = m_types.top();
            m_types.pop();
            if (pt) {
                m_binder.add_type(m->type_index(), pt);
                if (pt->is_subtype()) {
                    m_types.top()->add_type(pt);
                }
            }
        }

        void enum_value(const std::string& name, int64_t value)
        {
            m_types.top()->add_enum_value(name, value);
        }

        python_binder&              m_binder;
        std::stack<python_type_ref> m_types;
    };

    class type_fields_creator : public mge::script::visitor
    {
    public:
        type_fields_creator(python_binder& binder)
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
                m_types.push(m_binder.get_type(t->type_index()));
            } else {
                m_types.push(python_type_ref());
            }
        }

        void field(const std::string&                   name,
                   const mge::script::type_details_ref& type,
                   const mge::script::invoke_function&  setter,
                   const mge::script::invoke_function&  getter) override
        {
            if (current_type()) {
                current_type()->add_field(name, type, setter, getter);
            }
        }

        void
        constructor(const mge::script::signature&       s,
                    const mge::script::invoke_function& new_at,
                    const mge::script::invoke_function& make_shared) override
        {
            if (current_type()) {
                current_type()->add_constructor(s, new_at, make_shared);
            }
        }

        void destructor(
            const mge::script::invoke_function& delete_ptr,
            const mge::script::invoke_function& delete_shared_ptr) override
        {
            if (current_type()) {
                current_type()->add_destructor(delete_ptr, delete_shared_ptr);
            }
        }

        void finish(const mge::script::type_details_ref& m) override
        {
            m_types.pop();
        }

        void method(const std::string&                  name,
                    bool                                is_static,
                    const std::type_index&              return_type,
                    const mge::script::signature&       sig,
                    const mge::script::invoke_function& invoke) override
        {
            if (current_type()) {
                if (is_static) {
                    current_type()->add_static_method(name,
                                                      return_type,
                                                      sig,
                                                      invoke);
                } else {
                    current_type()->add_method(name, return_type, sig, invoke);
                }
            }
        }

    private:
        python_type_ref current_type()
        {
            if (m_types.empty()) {
                return python_type_ref();
            } else {
                return m_types.top();
            }
        }

        python_binder&                m_binder;
        std::stack<python_module_ref> m_py_modules;
        std::stack<python_type_ref>   m_types;
    };

    class type_finalizer : public mge::script::visitor
    {
    public:
        type_finalizer(python_binder& binder)
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
                m_types.push(m_binder.get_type(t->type_index()));
            } else {
                m_types.push(python_type_ref());
            }
        }

        void finish(const mge::script::type_details_ref& m) override
        {
            if (!m_types.empty() && m_types.top()) {
                m_py_modules.top()->add_type(m_types.top());
                m_types.pop();
            }
        }

    private:
        python_binder&                m_binder;
        std::stack<python_module_ref> m_py_modules;
        std::stack<python_type_ref>   m_types;
    };

    python_binder::python_binder(python_context& context)
        : m_context(context)
    {
        prologue();
    }

    void python_binder::bind(const mge::script::module& m)
    {
        MGE_DEBUG_TRACE(PYTHON) << "Binding module " << m.name();
        module_binder mb(*this);
        m.apply(mb);
        MGE_DEBUG_TRACE(PYTHON) << "Creating types of module " << m.name();
        type_creator tb(*this);
        m.apply(tb);
        MGE_DEBUG_TRACE(PYTHON)
            << "Creating fields of types of module " << m.name();
        type_fields_creator tc(*this);
        m.apply(tc);
        MGE_DEBUG_TRACE(PYTHON) << "Finalizing types of module " << m.name();
        type_finalizer tf(*this);
        m.apply(tf);
        MGE_DEBUG_TRACE(PYTHON)
            << "Run python initialization code for " << m.name();
        init();
    }

    void python_binder::add_init_code(const std::string& code)
    {
        m_init_code.emplace_back(code);
    }

    void python_binder::prologue()
    {
        const char* code = R"code(
import ctypes
        )code";
        PyRun_SimpleString(code);
        error::check_error();
    }

    void python_binder::init()
    {
        std::stringstream code;
        code << "def _():" << std::endl;
        code << "\tpass" << std::endl;
        for (const auto& codepiece : m_init_code) {
            for (auto it = boost::make_split_iterator(
                     codepiece,
                     boost::first_finder("\n", boost::is_equal()));
                 it != decltype(it)();
                 ++it) {
                code << "\t" << std::string_view(*it) << std::endl;
            }
        }
        code << std::endl << "_()" << std::endl << "del _" << std::endl;
        PyRun_SimpleString(code.str().c_str());
        try {
            error::check_error();
            m_init_code.clear();
        } catch (...) {
            MGE_ERROR_TRACE(PYTHON) << "Error executing init code: ";
            MGE_ERROR_TRACE(PYTHON) << code.str();
            m_init_code.clear();
            throw;
        }
    }
} // namespace mge::python