// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/reflection_fwd.hpp"
#include "mge/reflection/type.hpp"

#include <string>
#include <string_view>

namespace mge::reflection {

    class MGEREFLECTION_EXPORT module
    {
    public:
        module();
        module(const std::string_view name);
        ~module() = default;
        module(const module&) = default;
        module& operator=(const module&) = default;
        module(module&&) = default;
        module& operator=(module&&) = default;

        bool                    is_root() const;
        std::string_view        name() const;
        std::string             full_name() const;
        mge::reflection::module parent() const;

        template <typename T, typename... Args>
        inline module& operator()(T&& t, Args&&... args)
        {
            add(t);
            return operator()(std::forward<Args>(args)...);
        }

        inline module& operator()()
        {
            return *this;
        }

    private:
        template <typename T> void add(type<T>&& t)
        {
            // Implementation for adding the type information to the module
        }

        void add(const type_details_ref& details);

        module(const module_details_ref& details) :m_details(details) {}
        module_details_ref m_details;
    };

} // namespace mge::reflection
