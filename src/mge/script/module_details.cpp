// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/module_details.hpp"
#include "boost/boost_algorithm_string.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/type.hpp"
#include "mge/script/type_details.hpp"

namespace mge {
    MGE_USE_TRACE(SCRIPT);
}

namespace mge::script {

    module_details module_details::s_root_module;

    module_details::module_details(const std::string& name)
        : m_name(name)
        , m_parent(nullptr)
    {
        MGE_DEBUG_TRACE(SCRIPT) << "Create module '" << name << "'";
    }

    module_details::module_details()
        : m_parent(nullptr)
    {
        MGE_DEBUG_TRACE(SCRIPT) << "Create root module";
    }

    module_details::~module_details()
    {
        for (const auto& [n, m] : m_modules) {
            delete m;
        }
    }

    module_details* module_details::get(const std::string& path)
    {
        if (path.empty()) {
            return &s_root_module;
        }

        std::vector<std::string> modules;
        boost::iter_split(modules, path, boost::algorithm::first_finder("::"));

        module_details* current_module = &s_root_module;
        for (const auto& modulename : modules) {
            current_module = current_module->get_or_add_module(modulename);
        }

        return current_module;
    }

    module_details* module_details::get_or_add_module(const std::string& name)
    {
        auto it = m_modules.find(name);
        if (it != m_modules.end()) {
            return it->second;
        } else {
            auto new_module = std::make_unique<module_details>(name);
            new_module->m_parent = this;
            module_details* result = new_module.get();
            m_modules[name] = new_module.get();
            new_module.release();
            return result;
        }
    }

    std::string module_details::full_name() const
    {
        if (is_root()) {
            return "";
        } else {
            return m_parent->full_name() + "::" + name();
        }
    }

    void module_details::apply(visitor& v) {}

    void module_details::add_module(module_details* m)
    {
        if (m == this) {
            MGE_THROW(mge::illegal_state) << "Cannot add module to itself";
        }

        if (m->parent() == this) {
            return;
        } else if (m->parent() != &s_root_module) {
            MGE_THROW(mge::illegal_state)
                << "Cannot add module to parent which already has a parent";
        } else {
            m->m_parent->m_modules.erase(m->m_name);
            m->m_parent = this;
            m_modules[m->m_name] = m;
        }
    }

    void module_details::add_type(type_details* d)
    {
        auto ti = d->type_index();
        if (m_types.find(ti) != m_types.end()) {
            m_types.insert(std::make_pair(ti, d));
            d->set_module(this);
        }
    }

    void module_details::add_type(type_base& t) { add_type(t.details()); }

} // namespace mge::script