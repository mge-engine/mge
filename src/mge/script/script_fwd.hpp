// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/memory.hpp"
namespace mge {

    MGE_DECLARE_REF(script_engine);
    MGE_DECLARE_REF(script_context);

    namespace script {
        MGE_DECLARE_REF(module);
        MGE_DECLARE_WEAK_REF(module);
        MGE_DECLARE_REF(type);
        class visitor;
    } // namespace script

} // namespace mge