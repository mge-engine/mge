// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/module_details.hpp"

#include "boost/boost_algorithm_string.hpp"

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

    module_details_ref module_details::get(std::string_view name)
    {
        using namespace std::literals::string_view_literals;

        if (name.empty() || name == "::"sv) {
            return root();
        }
        std::vector<std::string> parts;
        boost::split(parts,
                     name,
                     boost::is_any_of("::"),
                     boost::token_compress_on);

        auto current = root();
        for (const auto& part : parts) {
            if (part.empty()) {
                continue;
            }
            auto it = std::find_if(current->m_children.begin(),
                                   current->m_children.end(),
                                   [&part](const module_details_ref& child) {
                                       return child->m_name == part;
                                   });
            if (it == current->m_children.end()) {
                auto new_child =
                    std::make_shared<module_details>(current, part);
                current = new_child;
            } else {
                current = *it;
            }
        }
        return current;
    }

    std::string module_details::full_name() const
    {
        if (m_parent.lock()) {
            return m_parent.lock()->full_name() + "::" + m_name;
        }
        return m_name;
    }

    bool module_details::is_root() const noexcept
    {
        return m_name.empty();
    }

} // namespace mge::reflection
