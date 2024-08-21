// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/memory.hpp"
namespace mge {
    MGE_DECLARE_REF(script_engine);
    MGE_DECLARE_REF(script_context);

    namespace script {
        class module;
        class type;
        template <typename R, typename... Args> class function;
        class method;
        class constructor;
        class destructor;

        MGE_DECLARE_REF(module_data);
        MGE_DECLARE_REF(type_data);
        MGE_DECLARE_REF(function_data);
        MGE_DECLARE_REF(method_data);
        MGE_DECLARE_REF(constructor_data);
        MGE_DECLARE_REF(destructor_data);
        MGE_DECLARE_WEAK_REF(module_data);
    } // namespace script
} // namespace mge