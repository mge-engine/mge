// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/memory.hpp"
#include <typeindex>
#include <vector>

namespace mge {

    MGE_DECLARE_REF(script_engine);
    MGE_DECLARE_REF(script_context);

    namespace script {
        class visitor;
        struct traits;

        class module;
        MGE_DECLARE_REF(module_details);
        MGE_DECLARE_WEAK_REF(module_details);
        MGE_DECLARE_REF(type_details);
        MGE_DECLARE_REF(enum_type_details);
        MGE_DECLARE_REF(function_details);
        class type_base;
    } // namespace script

} // namespace mge