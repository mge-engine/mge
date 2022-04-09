// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/module_details.hpp"
#include "boost/boost_algorithm_string.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/function.hpp"
#include "mge/script/function_details.hpp"
#include "mge/script/type.hpp"
#include "mge/script/type_details.hpp"
#include "mge/script/visitor.hpp"

namespace mge {
    MGE_USE_TRACE(SCRIPT);
}

namespace mge::script {

    struct root_module_holder
    {
        root_module_holder()
        {
            root_module = std::make_shared<module_details>();
        }

        module_details_ref root_module;
    };

    static singleton<root_module_holder> s_root_module_holder;

    module_details::module_details(const std::string& name)
        : m_name(name)
    {
        MGE_DEBUG_TRACE(SCRIPT) << "Create module '" << name << "'";
    }

    module_details::module_details()
    {
        MGE_DEBUG_TRACE(SCRIPT) << "Create root module";
    }

    module_details::~module_details() {}

    module_details_ref module_details::get(const std::string& path)
    {
        if (path.empty()) {
            return s_root_module_holder->root_module;
        }

        std::vector<std::string> modules;
        boost::iter_split(modules, path, boost::algorithm::first_finder("::"));

        module_details_ref current_module = s_root_module_holder->root_module;
        for (const auto& modulename : modules) {
            current_module = current_module->get_or_add_module(modulename);
        }

        return current_module;
    }

    module_details_ref
    module_details::get_or_add_module(const std::string& name)
    {
        auto it = m_modules.find(name);
        if (it != m_modules.end()) {
            return it->second;
        } else {
            auto new_module = std::make_shared<module_details>(name);
            new_module->m_parent = shared_from_this();
            m_modules[name] = new_module;
            return new_module;
        }
    }

    std::string module_details::full_name() const
    {
        if (is_root()) {
            return "";
        } else {
            return m_parent.lock()->full_name() + "::" + name();
        }
    }

    void module_details::apply(visitor& v)
    {
        auto self = shared_from_this();
        v.start(self);
        for (const auto& [ti, t] : m_types) {
            t->apply(v);
        }
        for (const auto& [n, mod] : m_modules) {
            mod->apply(v);
        }
        v.finish(self);
    }

    void module_details::add_module(module_details_ref& m)
    {
        if (m.get() == this) {
            MGE_THROW(mge::illegal_state) << "Cannot add module to itself";
        }

        if (m->parent().get() == this) {
            return;
        } else if (m->parent().get() !=
                   s_root_module_holder->root_module.get()) {
            MGE_THROW(mge::illegal_state)
                << "Cannot add module to parent which already has a parent";
        } else {
            if (!m->m_parent.expired()) {
                m->m_parent.lock()->m_modules.erase(m->m_name);
            }
            m->m_parent = shared_from_this();
            m_modules[m->m_name] = m;
        }
    }

    void module_details::add_type(type_details_ref& d)
    {
        auto ti = d->type_index();
        if (m_types.find(ti) == m_types.end()) {
            m_types.insert(std::make_pair(ti, d));
            d->set_module(shared_from_this());
        }
    }

    void module_details::add_function(function_details_ref& f)
    {
        const auto& n = f->name();
        if (m_functions.find(n) == m_functions.end()) {
            m_functions.insert(std::make_pair(n, f));
            f->set_module(shared_from_this());
        }
    }

    void module_details::add_function(function_base& f)
    {
        add_function(f.details());
    }

    void module_details::add_type(type_base& t) { add_type(t.details()); }

} // namespace mge::script