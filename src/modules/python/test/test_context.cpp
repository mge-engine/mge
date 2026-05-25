// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "pythontest.hpp"

namespace mge::python {
    class test_context : public pythontest
    {};

    TEST_F(test_context, create)
    {
        auto context = engine->create_context();
    }

    TEST_F(test_context, simple_eval)
    {
        auto context = engine->create_context();
        context->eval("print('Hello from Python')\n");
    }

    TEST_F(test_context, simple_exception)
    {
        auto context = engine->create_context();
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

    TEST_F(test_context, main_dash_c_succeeds)
    {
        auto        context = engine->create_context();
        const char* argv[]  = {"mgepython", "-c", "pass"};
        int         rc      = context->main(3, argv);
        EXPECT_EQ(0, rc);
    }

    TEST_F(test_context, main_dash_c_prints)
    {
        auto        context = engine->create_context();
        const char* argv[]  = {"mgepython", "-c", "print('hello from main')"};
        int         rc      = context->main(3, argv);
        EXPECT_EQ(0, rc);
    }

    TEST_F(test_context, main_dash_c_syntax_error_returns_nonzero)
    {
        auto        context = engine->create_context();
        const char* argv[]  = {"mgepython", "-c", "def ("};
        int         rc      = context->main(3, argv);
        EXPECT_NE(0, rc);
    }

} // namespace mge::python