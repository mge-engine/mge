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

        class module;
        class module_details;
    } // namespace script

} // namespace mge