/*
 * Copyright (c) 2017 by Alexander Schroeder
 */
#include "test/googletest.hpp"
#include "mge/core/executable_name.hpp"

TEST(executable_name, correct_name)
{
    std::string name(mge::executable_name());
    EXPECT_EQ(std::string("coretest"), name);
}
