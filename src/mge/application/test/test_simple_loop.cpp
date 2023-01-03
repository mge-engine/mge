// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/application/loop.hpp"
#include "mge/core/component.hpp"
#include "mock_loop_target.hpp"

using namespace testing;

TEST(simple_loop, create)
{
    auto l = mge::loop::create("simple");
    EXPECT_TRUE(l);
}

TEST(simple_loop, no_loop_is_quit)
{
    auto             l = mge::loop::create("simple");
    MOCK_loop_target t;
    EXPECT_CALL(t, is_quit()).Times(1).WillOnce(Return(true));
    l->run(t);
}

TEST(simple_loop, one_successful_loop)
{
    auto             l = mge::loop::create("simple");
    MOCK_loop_target t;
    EXPECT_CALL(t, is_quit())
        .Times(2)
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    {
        InSequence s;

        EXPECT_CALL(t, input(_));
        EXPECT_CALL(t, update(_, _, _));
        EXPECT_CALL(t, present(_, 0.0));
    }
    l->run(t);
}
