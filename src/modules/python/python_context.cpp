// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "python_context.hpp"
#include "python_engine.hpp"
#include "mge/core/trace.hpp"

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {

    python_context::python_context(python_engine* engine)
        : m_engine(engine)
    {
        MGE_INFO_TRACE(PYTHON, "Python context created");
    }

    python_context::~python_context() {}

    void python_context::eval(const std::string& script)
    {
        MGE_DEBUG_TRACE(PYTHON, "eval");
    }

    int python_context::main(int argc, const char** argv)
    {
        return 0;
    }

    void python_context::bind() {}

} // namespace mge::python
