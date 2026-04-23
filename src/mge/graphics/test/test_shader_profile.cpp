// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/shader_profile.hpp"
#include "test/googletest.hpp"
#include "test/test_stream_op.hpp"

TEST(shader_profile, default_constructor)
{
    mge::shader_profile p;
    EXPECT_TRUE(p.name().empty());
}

TEST(shader_profile, constructor)
{
    mge::semantic_version v(5, 0, 0);
    mge::shader_profile   p("hlsl", v);
    EXPECT_EQ("hlsl", p.name());
    EXPECT_EQ(v, p.version());
}

TEST(shader_profile, copy)
{
    mge::shader_profile p1("glsl", mge::semantic_version(4, 5, 0));
    mge::shader_profile p2(p1);
    EXPECT_EQ(p1, p2);
}

TEST(shader_profile, assignment)
{
    mge::shader_profile p1("glsl", mge::semantic_version(4, 5, 0));
    mge::shader_profile p2;
    p2 = p1;
    EXPECT_EQ(p1, p2);
}

TEST(shader_profile, equality)
{
    mge::shader_profile p1("hlsl", mge::semantic_version(5, 0, 0));
    mge::shader_profile p2("hlsl", mge::semantic_version(5, 0, 0));
    EXPECT_EQ(p1, p2);
}

TEST(shader_profile, inequality_name)
{
    mge::shader_profile p1("hlsl", mge::semantic_version(5, 0, 0));
    mge::shader_profile p2("glsl", mge::semantic_version(5, 0, 0));
    EXPECT_NE(p1, p2);
}

TEST(shader_profile, inequality_version)
{
    mge::shader_profile p1("hlsl", mge::semantic_version(5, 0, 0));
    mge::shader_profile p2("hlsl", mge::semantic_version(4, 0, 0));
    EXPECT_NE(p1, p2);
}

TEST(shader_profile, compatible_same)
{
    mge::shader_profile p1("hlsl", mge::semantic_version(5, 0, 0));
    mge::shader_profile p2("hlsl", mge::semantic_version(5, 0, 0));
    EXPECT_TRUE(p1.compatible(p2));
}

TEST(shader_profile, compatible_lower)
{
    mge::shader_profile p1("hlsl", mge::semantic_version(4, 0, 0));
    mge::shader_profile p2("hlsl", mge::semantic_version(5, 0, 0));
    EXPECT_TRUE(p1.compatible(p2));
}

TEST(shader_profile, not_compatible_higher)
{
    mge::shader_profile p1("hlsl", mge::semantic_version(6, 0, 0));
    mge::shader_profile p2("hlsl", mge::semantic_version(5, 0, 0));
    EXPECT_FALSE(p1.compatible(p2));
}

TEST(shader_profile, not_compatible_different_name)
{
    mge::shader_profile p1("hlsl", mge::semantic_version(5, 0, 0));
    mge::shader_profile p2("glsl", mge::semantic_version(5, 0, 0));
    EXPECT_FALSE(p1.compatible(p2));
}

TEST(shader_profile, stream_output)
{
    mge::shader_profile p("hlsl", mge::semantic_version(5, 0, 0));
    std::ostringstream  oss;
    oss << p;
    EXPECT_EQ("hlsl 5", oss.str());
}

TEST(shader_profile, format)
{
    mge::shader_profile p("glsl", mge::semantic_version(4, 5, 0));
    auto                s = fmt::format("{}", p);
    EXPECT_EQ("glsl 4.5", s);
}
