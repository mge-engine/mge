// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "pythontest.hpp"

namespace mge::python {
    class test_context : public pythontest
    {};

    TEST_F(test_context, create) { auto context = engine->create_context(); }

    TEST_F(test_context, simple_eval)
    {
        auto context = engine->create_context();
        context->eval("print('Hello from Python')\n");
    }

} // namespace mge::python