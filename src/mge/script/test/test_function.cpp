// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/function.hpp"
#include "mge/script/module.hpp"
#include "test/googletest.hpp"

void test_function_1(void) {}

TEST(function, void_c_function)
{
    using mge::script::function;

    mge::script::module m;
    m(function("test_function_1", test_function_1));
}
