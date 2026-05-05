// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "python_engine.hpp"
#include "python_context.hpp"

#include "mge/core/trace.hpp"

namespace mge {
    MGE_DEFINE_TRACE(PYTHON);
}

namespace mge::python {

    python_engine::python_engine()
    {
        MGE_INFO_TRACE(PYTHON, "Python script engine created");
    }

    python_engine::~python_engine()
    {
        MGE_INFO_TRACE(PYTHON, "Python script engine destroyed");
    }

    script_context_ref python_engine::create_context()
    {
        return std::make_shared<python_context>(this);
    }

    MGE_REGISTER_IMPLEMENTATION(python_engine, mge::script_engine, python);

} // namespace mge::python
