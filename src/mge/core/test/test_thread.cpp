// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"

#include "mge/core/stacktrace.hpp"
#include "mge/core/thread.hpp"

#include <chrono>

TEST(thread, unstarted_thread_not_joinable)
{
    mge::thread t;
    EXPECT_FALSE(t.joinable());
}

TEST(thread, simple_start_join)
{
    bool        thread_did_run = false;
    mge::thread t;
    t.start([&] {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        thread_did_run = true;
    });
    t.join();
    EXPECT_TRUE(thread_did_run);
}

void thread_wait_for_stacktrace()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

TEST(thread, get_stacktrace)
{
    mge::thread t;
    t.start([] { thread_wait_for_stacktrace(); });
    mge::stacktrace bt(t);
    t.join();
    bool function_found = false;
    for (const auto e : bt) {
        if (strstr(e.function, "thread_wait_for_stacktrace") == 0) {
            function_found = true;
            break;
        }
    }

    EXPECT_TRUE(function_found)
        << "Function not found in stacktrace: " << std::endl
        << bt;
}
