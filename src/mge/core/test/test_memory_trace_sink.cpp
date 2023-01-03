// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/memory_trace_sink.hpp"
#include "test/googletest.hpp"

TEST(memory_trace_sink, capacity)
{
    mge::memory_trace_sink s;

    EXPECT_EQ(0u, s.capacity());
    s.set_capacity(122u);
    EXPECT_EQ(122u, s.capacity());
}