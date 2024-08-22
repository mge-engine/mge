// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/function.hpp"
#include "mge/script/module.hpp"
#include "mock_call_context.hpp"
#include "test/googletest.hpp"

int test_function_1_called = 0;

void test_function_1(void) { test_function_1_called = 42; }

TEST(function, void_c_function)
{
    using mge::script::function;

    mge::script::module m;
    m(function("test_function_1", test_function_1));
    MOCK_call_context ctx;
    m.function("test_function_1").invoke(ctx);
    EXPECT_EQ(42, test_function_1_called);
}
