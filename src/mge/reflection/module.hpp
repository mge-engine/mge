// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/reflection_fwd.hpp"

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

        bool is_root() const;
    private:
        module_details_ref m_details;
    };

} // namespace mge::reflection
