// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "python_engine.hpp"
#include "mge/core/trace.hpp"

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {

    python_engine::python_engine() {}
    python_engine::~python_engine() {}

    mge::script_context_ref python_engine::create_context()
    {
        return mge::script_context_ref();
    }
} // namespace mge::python