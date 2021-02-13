// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/stacktrace.hpp"
#include "test/googletest.hpp"

TEST(stacktrace, basic)
{
    auto stack               = mge::stacktrace();
    bool line_in_stack_found = false;
    for (const auto &f : stack) {
        if (f.name() == "mge::stacktrace::stacktrace") {
            line_in_stack_found = true;
            break;
        }
    }
    EXPECT_TRUE(line_in_stack_found)
        << "Did not find 'mge::stacktrace::stacktrace' in " << stack;
    EXPECT_GE(stack.size(), 2u);
}

TEST(stacktrace, copy_ctor)
{
    auto stack        = mge::stacktrace();
    auto copied_stack = mge::stacktrace(stack);
    EXPECT_TRUE(stack == copied_stack);
}

TEST(stacktrace, assignment)
{
    auto stack1 = mge::stacktrace();
    auto stack2 = mge::stacktrace();

    stack2 = stack1;
    EXPECT_TRUE(stack1 == stack2);
}