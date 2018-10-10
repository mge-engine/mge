// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "mge/core/task_queue.hpp"


TEST(task_queue, empty)
{
    mge::task_queue q;
    EXPECT_TRUE(q.empty());
}

TEST(task_queue, size)
{
    mge::task_queue q;
    q.push_back(std::make_shared<mge::task>([]{}));
    EXPECT_EQ(q.size(), 1u);
}
