// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/callable.hpp"
#include "test/googletest.hpp"
#include <functional>

namespace mge {

    void foo() {}

    TEST(callable, is_function)
    {
        EXPECT_TRUE(std::is_function<decltype(foo)>::value);
    }

    TEST(callable, callable)
    {
        auto x = [](int x) {};
        auto y = []() -> int { return 12; };

        std::function<int(void)> f(y);

        EXPECT_TRUE(mge::is_callable<decltype(x)>::value);
        EXPECT_TRUE(mge::is_callable<decltype(y)>::value);
        EXPECT_FALSE(mge::is_callable<int>::value);
        EXPECT_FALSE(mge::is_callable<decltype(foo)>::value);
        EXPECT_TRUE(mge::is_callable<decltype(f)>::value);
    }

    template <callable C> int bar(C c) { return 42; }

    TEST(callable, template_callable)
    {
        auto x = [](int x) {};
        auto y = []() -> int { return 12; };

        EXPECT_EQ(42, bar(x));
        EXPECT_EQ(42, bar(y));
    }

} // namespace mge
