// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/task_queue.hpp"
#include "test/googletest.hpp"

TEST(task_queue, empty)
{
    mge::task_queue q;
    EXPECT_TRUE(q.empty());
}

TEST(task_queue, size)
{
    mge::task_queue q;
    q.push_back(std::make_shared<mge::task>([] {}));
    EXPECT_EQ(q.size(), 1u);
}

TEST(task_queue, move)
{
    mge::task_queue q;
    q.push_back(std::make_shared<mge::task>([] {}));
    mge::task_queue q2(std::move(q));
    EXPECT_EQ(q2.size(), 1u);
}

TEST(task_queue, copy)
{
    mge::task_queue q;
    q.push_back(std::make_shared<mge::task>([] {}));
    mge::task_queue q2(q);
    EXPECT_EQ(q2.size(), 1u);
}

TEST(task_queue, pop_front)
{
    auto            tsk = std::make_shared<mge::task>([] {});
    mge::task_queue q;
    q.push_back(tsk);
    auto popped = q.pop_front();
    EXPECT_EQ(tsk.get(), popped.get());
}

TEST(task_queue, pop_front_empty)
{
    mge::task_queue q;
    auto            popped = q.pop_front();
    EXPECT_EQ(nullptr, popped.get());
}
