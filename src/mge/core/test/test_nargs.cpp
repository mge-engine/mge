// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/nargs.hpp"
#include "test/googletest.hpp"

TEST(nargs, with_args)
{
    auto n = MGE_NARGS(a);
    EXPECT_EQ(1, n);
    auto n2 = MGE_NARGS(a, b, c);
    EXPECT_EQ(3, n2);
    auto n3 = MGE_NARGS(a1, a2, a3, a4, a5, a6, a6, a8, a9, a10, a11, a12);
    EXPECT_EQ(12, n3);
    auto n4 = MGE_NARGS(a1, a2, a3, a4, a5, a6, a6, a8, a9, a10, a11, a12, a13);
    EXPECT_EQ(13, n4);
}

TEST(nargs, without_args)
{
    auto n = MGE_NARGS();
    EXPECT_EQ(1, n);
}

TEST(nargs, has_comma)
{
    auto hc = MGE_HAS_COMMA(a);
    EXPECT_EQ(0, hc);
    auto hc2 = MGE_HAS_COMMA();
    EXPECT_EQ(0, hc2);
    auto hc3 = MGE_HAS_COMMA(a, b);
    EXPECT_EQ(1, hc3);
}

TEST(nargs, is_empty)
{
    auto is_empty = MGE_IS_EMPTY(aaa);
    auto is_empty2 = MGE_IS_EMPTY(aaa, bbb);
    auto is_empty3 = MGE_IS_EMPTY();
    EXPECT_EQ(0, is_empty);
    EXPECT_EQ(0, is_empty2);
    EXPECT_EQ(1, is_empty3);
}