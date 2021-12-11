// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/module_details.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/type_details.hpp"
#include "mge/script/variable_details.hpp"
#include "mge/script/visitor.hpp"

#include <mutex>

namespace mge {
    MGE_USE_TRACE(SCRIPT);
}

namespace mge::script {

    module_details::module_details(const module_details_ref& parent,
                                   const std::string&        name)
        : m_parent(parent)
        , m_name(name)
    {}

    module_details::~module_details() {}

    const std::string& module_details::name() const { return m_name; }

    bool module_details::is_root() const
    {
        return m_parent.expired() && m_name.empty();
    }

    module_details_ref module_details::parent() const
    {
        return m_parent.lock();
    }

    static std::once_flag     s_root_module_initialized;
    static module_details_ref s_root_module;

    module_details_ref module_details::root()
    {
        std::call_once(s_root_module_initialized, [] {
            s_root_module =
                std::make_shared<module_details>(module_details_ref(), "");
        });
        return s_root_module;
    }

    module_details_ref module_details::get_or_add_child(const std::string& name)
    {
        auto it = m_children.find(name);
        if (it != m_children.end()) {
            return it->second;
        } else {
            MGE_DEBUG_TRACE(SCRIPT) << "Add new module " << name;
            auto new_details =
                std::make_shared<module_details>(shared_from_this(), name);
            m_children[name] = new_details;
            return new_details;
        }
    }

    void module_details::add_child(const module_details_ref& child)
    {
        m_children[child->name()] = child;
        child->m_parent = shared_from_this();
    }

    void module_details::add_type(const type_details_ref& child)
    {
        m_types[child->name()] = child;
        child->m_module = shared_from_this();
    }

    void module_details::add_variable(const variable_details_ref& child)
    {
        m_variables[child->name()] = child;
        child->m_module = shared_from_this();
    }

    void module_details::apply(visitor& v)
    {
        v.begin(*this);

        for (const auto& [key, value] : m_types) {
            value->apply(v);
        }

        for (const auto& [key, value] : m_variables) {
            value->apply(v);
        }

        for (const auto& [key, value] : m_children) {
            value->apply(v);
        }

        v.end(*this);
    }

} // namespace mge::script