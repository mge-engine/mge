// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "mge/core/tagged.hpp"
#include <iostream>

using namespace mge;

struct tag_type {};
struct tag2_type {};

class nondefault_ctor {
public:
    nondefault_ctor(int) {}
};

TEST(tagged_value, nondefault_ctor)
{
    mge::tagged<nondefault_ctor, tag_type> x(43);
}


TEST(tagged_value, default_ctor)
{
    mge::tagged<int, tag_type> x;
}

TEST(tagged_value, other_ctor)
{
    mge::tagged<int, tag_type> x(17);
}

TEST(tagged_value, copy_tagged)
{
    mge::tagged<int, tag_type> x(17);
    mge::tagged<int, tag2_type> y(x);

    EXPECT_EQ(y.get(), 17);
}
