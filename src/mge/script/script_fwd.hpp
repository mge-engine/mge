// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/memory.hpp"
namespace mge {

    MGE_DECLARE_REF(script_engine);
    MGE_DECLARE_REF(script_context);

    namespace script {
        class visitor;
        class module;
        struct type_classification;
        struct field_details;
        class call_context;
        template <typename T, typename> class type;
        template <typename T> class variable;

        /*
        class constructor;
        class destructor;
        class method;
        */

        MGE_DECLARE_REF(module_details);
        MGE_DECLARE_WEAK_REF(module_details);

        MGE_DECLARE_REF(type_details);
        MGE_DECLARE_WEAK_REF(type_details);
        MGE_DECLARE_REF(enum_type_details);
        MGE_DECLARE_WEAK_REF(enum_type_details);
        MGE_DECLARE_REF(class_type_details);
        MGE_DECLARE_WEAK_REF(class_type_details);

        MGE_DECLARE_REF(variable_details);
        MGE_DECLARE_WEAK_REF(variable_details);

        MGE_DECLARE_REF(function_details);
        MGE_DECLARE_WEAK_REF(function_details);
    } // namespace script

} // namespace mge