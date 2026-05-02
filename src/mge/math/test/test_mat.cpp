// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/format.hpp"
#include "mge/math/mat.hpp"
#include "test/googletest.hpp"
#include <sstream>

using namespace mge;

TEST(mat2, print)
{
    fmat2             m(1.0f);
    std::stringstream ss;
    ss << m;
    EXPECT_STREQ("[[1, 0], [0, 1]]", ss.str().c_str());
}

TEST(mat2, format)
{
    EXPECT_EQ("[[1, 0], [0, 1]]", fmt::format("{}", fmat2(1.0f)));
    EXPECT_EQ("[[1, 0], [0, 1]]", fmt::format("{}", dmat2(1.0)));
}

TEST(mat3, print)
{
    fmat3             m(1.0f);
    std::stringstream ss;
    ss << m;
    EXPECT_STREQ("[[1, 0, 0], [0, 1, 0], [0, 0, 1]]", ss.str().c_str());
}

TEST(mat3, format)
{
    EXPECT_EQ("[[1, 0, 0], [0, 1, 0], [0, 0, 1]]",
              fmt::format("{}", fmat3(1.0f)));
    EXPECT_EQ("[[1, 0, 0], [0, 1, 0], [0, 0, 1]]",
              fmt::format("{}", dmat3(1.0)));
}

TEST(mat4, print)
{
    fmat4             m(1.0f);
    std::stringstream ss;
    ss << m;
    EXPECT_STREQ("[[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]]",
                 ss.str().c_str());
}

TEST(mat4, format)
{
    EXPECT_EQ("[[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]]",
              fmt::format("{}", fmat4(1.0f)));
    EXPECT_EQ("[[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]]",
              fmt::format("{}", dmat4(1.0)));
}
