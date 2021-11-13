// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/module_details.hpp"
#include <mutex>

namespace mge::script {

    module_details::module_details(const module_details_ref& parent,
                                   const std::string_view    name)
        : m_parent(parent)
        , m_name(name)
    {}

    module_details::~module_details() {}

    std::string_view module_details::name() const { return m_name; }

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

    module_details_ref
    module_details::get_or_add_child(const std::string_view name)
    {
        auto it = m_children.find(name);
        if (it != m_children.end()) {
            return it->second;
        } else {
            auto new_details =
                std::make_shared<module_details>(shared_from_this(), name);
            m_children[new_details->name()] = new_details;
            return new_details;
        }
    }

    void module_details::add_child(const module_details_ref& child)
    {
        m_children[child->name()] = child;
        child->m_parent = shared_from_this();
    }

} // namespace mge::script