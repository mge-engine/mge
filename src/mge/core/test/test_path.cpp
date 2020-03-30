// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/path.hpp"
#include "test/googletest.hpp"

using mge::path;

TEST(path, construct)
{
    path p("c:/wurst/salat");
    EXPECT_EQ(std::string("c:/wurst/salat"), p.generic_string());
}

TEST(path, native)
{
#ifdef MGE_OS_WINDOWS
    path p("c:/wurst/salat");
    p.make_preferred();
    EXPECT_EQ(std::string("c:\\wurst\\salat"), p.string());
#else
    path p("brot/wurst");
    EXPECT_EQ(std::string("brot/wurst"), p.string());
    path p2("/usr/brot/wurst");
    EXPECT_EQ(std::string("/usr/brot/wurst"), p2.string());
#endif
}

TEST(path, is_absolute)
{
#ifdef MGE_OS_WINDOWS
    path p("c:/wurst");
    EXPECT_TRUE(p.is_absolute());
#else
    path p("/wurst");
    EXPECT_TRUE(p.is_absolute());
#endif
    path p2("brot");
    EXPECT_FALSE(p2.is_absolute());
}

TEST(path, extension)
{
    path p("foo.bar");
    EXPECT_STREQ(".bar", p.extension().string().c_str());
}

TEST(path, append_to_itself)
{
    path p("wurst");
    p /= p;
    EXPECT_EQ(std::string("wurst/wurst"), p.generic_string());
}
