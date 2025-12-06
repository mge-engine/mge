// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/reflection_fwd.hpp"

#include <string>
#include <vector>

namespace mge::reflection {

    class MGEREFLECTION_EXPORT module_details
        : public std::enable_shared_from_this<module_details>
    {
    public:
        module_details(const module_details_ref& parent, std::string_view name);
        ~module_details() = default;

        static module_details_ref root();
        static module_details_ref get(std::string_view name);

        const auto& name() const noexcept
        {
            return m_name;
        }
        const auto& children() const noexcept
        {
            return m_children;
        }

        std::string full_name() const;
        bool        is_root() const noexcept;

        module_details_ref parent() const noexcept
        {
            return m_parent.lock();
        }

        void add(const type_details_ref& details);
        void add(const function_details_ref& details);
        void add(const module_details_ref& details);

    private:
        module_details_weak_ref         m_parent;
        std::string                     m_name;
        std::vector<module_details_ref> m_children;

        static module_details_ref s_root;
    };
} // namespace mge::reflection
