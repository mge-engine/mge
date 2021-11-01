// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/script_context.hpp"
#include "python.hpp"

namespace mge::python {
    class python_context : public script_context
    {
    public:
        python_context();
        ~python_context();
        void eval(std::string_view code);
    };
} // namespace mge::python