// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "mge/core/path.hpp"

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
    EXPECT_EQ(std::string("c:\\wurst\\salat"), p.native());
#else
    path p("brot/wurst");
    EXPECT_EQ(std::string("brot/wurst"), p.native());
    path p2("/usr/brot/wurst");
    EXPECT_EQ(std::string("/usr/brot/wurst"), p2.native());
#endif
}

TEST(path, is_absolute)
{
#ifdef MGE_OS_WINDOWS
    path p("c:/wurst");
    EXPECT_TRUE(p.absolute());
#else
    path p("/wurst");
    EXPECT_TRUE(p.absolute());
#endif
    path p2("brot");
    EXPECT_FALSE(p2.absolute());
}

TEST(path, extension)
{
    path p("foo.bar");
    EXPECT_STREQ("bar", p.extension().c_str());
}

TEST(path, append_to_itself)
{
    path p("wurst");
    p /= p;
    EXPECT_EQ(std::string("wurst/wurst"), p.generic_string());
}
