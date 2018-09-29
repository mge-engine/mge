// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "mge/core/stream_values.hpp"

#include <sstream>

TEST(stream_values, stream_some_values)
{
    std::stringstream ss;
    mge::stream_values(ss, "Mary ", "had ", 1, " little lamb");
    std::string s(ss.str());
    EXPECT_STREQ("Mary had 1 little lamb", s.c_str());
}
