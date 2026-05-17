// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/core/memory.hpp"

namespace mge::python {

    class python_engine;
    class python_context;
    class python_module;
    class python_binder;
    class python_type;
    class error;

    MGE_DECLARE_REF(python_module);
    MGE_DECLARE_REF(python_context);
    MGE_DECLARE_REF(python_engine);
    MGE_DECLARE_REF(python_binder);
    MGE_DECLARE_REF(python_type);
    MGE_DECLARE_REF(error);

} // namespace mge::python