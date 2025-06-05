// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/viewport.hpp"
#include "test/googletest.hpp"
#include "test/test_stream_op.hpp"

TEST(viewport, format)
{
    mge::viewport      v(10.0, 20.0, 30.0, 40.0, 0.0, 1.0);
    std::ostringstream oss;
    oss << v;
    EXPECT_EQ(
        "viewport{x=10, y=20, width=30, height=40, min_depth=0, max_depth=1}",
        oss.str());
}
