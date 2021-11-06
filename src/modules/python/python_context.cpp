// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "python_context.hpp"
#include "python_error.hpp"

namespace mge::python {
    python_context::python_context() {}

    python_context::~python_context() {}

    void python_context::eval(std::string_view code)
    {
        std::string code_str(code);
        PyRun_SimpleString(code_str.c_str());
    }

} // namespace mge::python