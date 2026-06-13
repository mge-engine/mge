// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/format.hpp"
#include "mge/math/vec4.hpp"
#include "test/googletest.hpp"
#include <sstream>

using namespace mge;

TEST(vec4, print)
{
    fvec4             v(1.0f, 2.0f, 3.0f, 4.0f);
    std::stringstream ss;
    ss << v;
    EXPECT_STREQ("(1, 2, 3, 4)", ss.str().c_str());
}

TEST(vec4, format)
{
    EXPECT_EQ("(1, 2, 3, 4)", fmt::format("{}", fvec4(1.0f, 2.0f, 3.0f, 4.0f)));
    EXPECT_EQ("(1, 2, 3, 4)", fmt::format("{}", ivec4(1, 2, 3, 4)));
    EXPECT_EQ("(1, 2, 3, 4)", fmt::format("{}", uvec4(1u, 2u, 3u, 4u)));
    EXPECT_EQ("(1, 2, 3, 4)", fmt::format("{}", dvec4(1.0, 2.0, 3.0, 4.0)));
}
