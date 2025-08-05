// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/module_details.hpp"

namespace mge::reflection {

    module_details_ref module_details::s_root =
        std::make_shared<module_details>(nullptr, "");

    module_details::module_details(const module_details_ref& parent,
                                   std::string_view          name)
        : m_parent(parent)
        , m_name(name)
    {
        if (parent) {
            parent->m_children.push_back(shared_from_this());
        }
    }

    module_details_ref module_details::root()
    {
        return s_root;
    }

    std::string module_details::full_name() const
    {
        if (m_parent.lock()) {
            return m_parent.lock()->full_name() + "::" + m_name;
        }
        return m_name;
    }

} // namespace mge::reflection
