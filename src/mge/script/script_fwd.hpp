// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/memory.hpp"
#include <functional>
#include <typeindex>
#include <vector>

namespace mge {

    MGE_DECLARE_REF(script_engine);
    MGE_DECLARE_REF(script_context);
    MGE_DECLARE_REF(script_binder);

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
        class function_base;
        class call_context;
        class signature;

        /**
         * Invocation function that uses a call context for
         * parameter retrieval and result storage.
         */
        using invoke_function = std::function<void(call_context&)>;
    } // namespace script

} // namespace mge