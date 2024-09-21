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
        void execute() override { simple_closure_executed = true; }
    };

    void_closure c;
    c.function()();
    EXPECT_TRUE(simple_closure_executed);
}

TEST(closure, simple_int)
{
    struct int_closure : public mge::closure<int, int>
    {
        int execute(int x) override { return x + 1; }
    };

    int_closure c;
    EXPECT_EQ(42, c.function()(41));
}