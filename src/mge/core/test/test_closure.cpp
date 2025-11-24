// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/closure.hpp"
#include "test/googletest.hpp"

static bool simple_closure_executed = false;

TEST(closure, simple)
{
    struct void_closure : public mge::closure<void, void>
    {
        void execute() override
        {
            simple_closure_executed = true;
        }
    };

    void_closure c;
    c.function()();
    EXPECT_TRUE(simple_closure_executed);
}

TEST(closure, simple_int)
{
    struct int_closure : public mge::closure<int, int>
    {
        int execute(int x) override
        {
            return x + 1;
        }
    };

    int_closure c;
    EXPECT_EQ(42, c.function()(41));
}

TEST(closure, three_params)
{
    struct int_closure : public mge::closure<int, int, int, int>
    {
        int execute(int x, int y, int z) override
        {
            return z + y * 10 + x * 100;
        }
    };

    int_closure c;
    EXPECT_EQ(123, c.function()(1, 2, 3));
}

TEST(closure, different_ints)
{
    struct int_closure : public mge::closure<int64_t, int16_t, int32_t, int64_t>
    {
        int64_t execute(int16_t x, int32_t y, int64_t z) override
        {
            return z + y * 10 + x * 100;
        }
    };

    int_closure c;
    EXPECT_EQ(123, c.function()(1, 2, 3));
}
