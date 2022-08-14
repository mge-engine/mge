// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/gist.hpp"
#include "test/googletest.hpp"

#include <sstream>

using namespace mge;

TEST(gist, pod)
{
    std::stringstream ss;
    ss << gist(2);
    EXPECT_EQ("2", ss.str());
}

struct no_gist_op
{};

TEST(gist, no_gist_op)
{
    no_gist_op        val;
    std::stringstream ss;
    ss << gist(val);

    EXPECT_TRUE(ss.str().starts_with("no_gist_op@"));
}

struct gist_op
{
    void gist(std::ostream& os) const { os << "gist op implemented"; }
};

TEST(gist, gist_op)
{
    gist_op           val;
    std::stringstream ss;
    ss << gist(val);

    EXPECT_EQ(ss.str(), "gist op implemented");
}
