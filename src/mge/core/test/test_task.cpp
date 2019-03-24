// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/task.hpp"

TEST(task, simple_run)
{
    bool executed = false;
    mge::task t([&]{
        executed = true;
    });
    t.run();
    EXPECT_TRUE(executed);
    t.wait();
}

TEST(task, exception_in_run)
{
    mge::task t([]{
        MGE_THROW(mge::runtime_exception) << "A runtime exception";
    });
    t.run();
    EXPECT_THROW(t.wait(), mge::runtime_exception);
}
