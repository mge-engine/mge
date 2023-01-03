// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/vertex_format.hpp"
#include "test/googletest.hpp"
#include "test/test_stream_op.hpp"

TEST(vertex_format, output)
{
    mge::vertex_format f(mge::data_type::FLOAT, 3);
    mge::test_stream_output(f, "float[3]");
}

TEST(vertex_format, parse)
{
    mge::vertex_format f(mge::data_type::FLOAT, 3);
    EXPECT_EQ(f, mge::parse_vertex_format("float[3]"));
}
