// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "test/googlemock.hpp"
#include "mge/core/gist.hpp"
#include <sstream>

using namespace mge;
using namespace testing;
using namespace std::string_literals;

class foobar {};


TEST(gist, gist_pod_type)
{
    std::stringstream ss;
    ss << gist(123);
    EXPECT_EQ("123"s, ss.str());
}

TEST(gist, gist_class_type)
{
    foobar foo;
    std::stringstream ss;
    ss << gist(foo);
    EXPECT_TRUE(ss.str().starts_with("foobar@"));

}