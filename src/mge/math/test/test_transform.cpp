// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/math/mat4.hpp"
#include "mge/math/transform.hpp"
#include "test/googletest.hpp"
#include <iostream>
using namespace mge;

TEST(transform, look_at)
{
    fvec3 eye{1.0, 1.0, 1.0};
    fvec3 center{0,0,0};
    fvec3 up{0,1,0};

    auto m = look_at(eye, center, up);
    EXPECT_EQ(1.0f, m[3][3]);
}
