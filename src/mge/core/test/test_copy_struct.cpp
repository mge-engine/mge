// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/copy_struct.hpp"
#include "test/googletest.hpp"

struct foo
{
    int    a;
    char   b[200];
    double c;
};

TEST(copy_struct, copy_struct)
{
    foo x1;
    x1.a = 17;
    strcpy(x1.b, "foobar");
    x1.c = 34.5;

    foo x2;
    mge::copy_struct(x2, x1);
    EXPECT_EQ(x1.a, x2.a);
    EXPECT_STREQ(x1.b, x2.b);
    EXPECT_EQ(x1.c, x2.c);
}