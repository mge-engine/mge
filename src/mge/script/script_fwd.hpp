// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
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
    MGE_DECLARE_REF(script_type);

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

        /**
         * @brief Function used for transfer of shared pointers.
         *
         * The first argument of the function is the target shared
         * pointer address, the second argument is the source shared
         * pointer address.
         */
        using transfer_shared = std::function<void(void*, void*)>;

    } // namespace script

} // namespace mge