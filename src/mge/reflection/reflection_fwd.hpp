// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/core/memory.hpp"
#include <functional>

namespace mge::reflection {

    MGE_DECLARE_REF(module_details);
    MGE_DECLARE_WEAK_REF(module_details);
    MGE_DECLARE_REF(type_details);
    MGE_DECLARE_REF(function_details);

    class call_context;
    using invoke_function = std::function<void(call_context&)>;

    template <typename T>
    inline const type_details_ref& get_or_create_type_details();

} // namespace mge::reflection
