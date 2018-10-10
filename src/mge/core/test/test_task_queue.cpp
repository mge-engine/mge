// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "mge/core/task_queue.hpp"


TEST(task_queue, is_lock_free)
{
    mge::task_queue q;
    EXPECT_TRUE(q.is_lock_free());
}
