// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "pythontest.hpp"

namespace mge::python {
    class test_context : public pythontest
    {};

    TEST_F(test_context, create)
    {
        auto ctx = engine->create_context();
    }

    TEST_F(test_context, simple_eval)
    {
        context->eval("print('Hello from Python')\n");
    }

    TEST_F(test_context, simple_exception)
    {
        try {
            context->eval(
                "raise Exception(\"My hoovercraft is full of eels\")");
            // "print('hello')");
            FAIL() << "Exception expected";
        } catch (const mge::exception& e) {
            EXPECT_STREQ("Python error (of <class 'Exception'>): My "
                         "hoovercraft is full of eels",
                         e.what());
        }
    }

} // namespace mge::python