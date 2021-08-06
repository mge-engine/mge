// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/executable_name.hpp"
#include "test/googletest.hpp"

TEST(executable_name, correct_name)
{
    std::string name(mge::executable_name());
    EXPECT_EQ(std::string("test_core"), name);
}
