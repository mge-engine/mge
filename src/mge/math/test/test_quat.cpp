// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/format.hpp"
#include "mge/math/quat.hpp"
#include "test/googletest.hpp"
#include <sstream>

using namespace mge;

TEST(quat, print)
{
    // fquat(w, x, y, z)
    fquat             q(1.0f, 0.0f, 0.0f, 0.0f);
    std::stringstream ss;
    ss << q;
    EXPECT_STREQ("(1, 0, 0, 0)", ss.str().c_str());
}

TEST(quat, format)
{
    EXPECT_EQ("(1, 0, 0, 0)", fmt::format("{}", fquat(1.0f, 0.0f, 0.0f, 0.0f)));
    EXPECT_EQ("(1, 0, 0, 0)", fmt::format("{}", dquat(1.0, 0.0, 0.0, 0.0)));
}
