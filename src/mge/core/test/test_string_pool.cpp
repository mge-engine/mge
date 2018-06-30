// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "mge/core/string_pool.hpp"

using mge::string_pool;

TEST(string_pool, construct)
{
    string_pool sp;
}

TEST(string_pool, intern_c_string)
{
    string_pool sp;

    const char *s1 = sp.intern("FOO");
    const char *s2 = sp.intern("FOO");

    EXPECT_STREQ(s1, "FOO");
    EXPECT_STREQ(s1, s2);
    EXPECT_EQ(s1, s2);
}

TEST(string_pool, intern_cxx_string)
{
    string_pool sp;
    std::string foo("FOO");
    const char *s1 = sp.intern(foo);
    const char *s2 = sp.intern(foo);

    EXPECT_STREQ(s1, "FOO");
    EXPECT_STREQ(s1, s2);
    EXPECT_EQ(s1, s2);
}
