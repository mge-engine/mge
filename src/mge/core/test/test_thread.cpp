// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/thread.hpp"
#include "test/googletest.hpp"
#include <string>

using namespace std::string_literals;

TEST(thread, generated_name)
{
    auto t = std::make_shared<mge::thread>();

    EXPECT_EQ("thread-1"s, t->name());
}

TEST(thread, construct)
{
    auto t = std::make_shared<mge::thread>("foobar"s);
    EXPECT_EQ("foobar"s, t->name());
}