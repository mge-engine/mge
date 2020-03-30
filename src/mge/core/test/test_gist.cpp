// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/gist.hpp"
#include "test/googlemock.hpp"
#include "test/googletest.hpp"
#include <sstream>

using namespace mge;
using namespace testing;
using namespace std::string_literals;

class foobar
{};

TEST(gist, gist_pod_type)
{
    std::stringstream ss;
    ss << gist(123);
    EXPECT_EQ("123"s, ss.str());
}

TEST(gist, gist_class_type)
{
    foobar            foo;
    std::stringstream ss;
    ss << gist(foo);
    EXPECT_TRUE(ss.str().starts_with("foobar@"));
}

struct foo
{
    void gist(std::ostream &os) const { os << "foo from gist"; }
};

struct bar : foo
{};

TEST(gist, gist_duck_typing)
{
    foo               f;
    std::stringstream ss;
    ss << gist(f);
    EXPECT_EQ("foo from gist"s, ss.str());
}

TEST(gist, gist_duck_typing_derived)
{
    bar               f;
    std::stringstream ss;
    ss << gist(f);
    EXPECT_EQ("foo from gist"s, ss.str());
}

TEST(gist, shared_ptr_derived)
{
    auto              b = std::make_shared<bar>();
    std::stringstream ss;
    ss << gist(b);
    EXPECT_EQ("std::shared_ptr<bar> => foo from gist"s, ss.str());
}