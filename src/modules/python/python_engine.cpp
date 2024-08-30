// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.

#include "python_engine.hpp"
#include "python_context.hpp"

#include "mge/config.hpp"
#include "mge/core/trace.hpp"

namespace mge {
    MGE_DEFINE_TRACE(PYTHON);
}

namespace mge::python {

    python_engine::python_engine()
    {
        MGE_INFO_TRACE(PYTHON) << "Creating Python script engine";
    }
    python_engine::~python_engine() {}

    mge::script_context_ref python_engine::create_context()
    {
        auto result = std::make_shared<python_context>(
            std::dynamic_pointer_cast<python_engine>(self().lock()));
        return result;
    }
} // namespace mge::python