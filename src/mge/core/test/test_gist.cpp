// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/config.hpp"
#include "mge/core/gist.hpp"
#include "test/googletest.hpp"

#include <sstream>

using namespace mge;

TEST(gist, arithmetic)
{
    int                 i = 2;
    mge::gist_type<int> gi = mge::gist(i);
    auto                s = std::format("{}", gi);
    EXPECT_STREQ("2", s.c_str());
}

TEST(gist, pointer)
{
    int                  i = 2;
    int*                 pi = &i;
    mge::gist_type<int*> gpi = mge::gist(pi);
    auto                 s = std::format("{}", gpi);
#if defined(MGE_COMPILER_MSVC)
    EXPECT_STREQ("int * -> 2", s.c_str());
#elif defined(MGE_COMPILER_GCC)
    EXPECT_STREQ("int* -> 2", s.c_str());
#endif
}

TEST(gist, shared_pointer)
{
    int                  i = 2;
    std::shared_ptr<int> spi = std::make_shared<int>(i);
    auto                 gspi = mge::gist(spi);
    auto                 s = std::format("{}", gspi);
    EXPECT_STREQ("std::shared_ptr<int> -> 2", s.c_str());
}

class has_gist
{
public:
    template <typename FormatContext> void gist(FormatContext& ctx) const
    {
        std::format_to(ctx.out(), "gist from has_gist");
    }
};

TEST(gist, has_gist)
{
    has_gist g;
    auto     gg = mge::gist(g);
    EXPECT_TRUE(mge::has_gist_method<has_gist>);
    auto s = std::format("{}", gg);
    EXPECT_STREQ("gist from has_gist", s.c_str());

    std::stringstream ss;
    ss << mge::gist(g);
    EXPECT_STREQ("gist from has_gist", ss.str().c_str());
}

struct no_gist
{};

TEST(gist, no_gist)
{
    no_gist ng;
    auto    gng = mge::gist(ng);
    auto    s = std::format("{}", gng);
    EXPECT_TRUE(s.starts_with("no_gist@"));
}