// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/program_options.hpp"
#include "test/googletest.hpp"

using namespace mge;

TEST(program_options, empty)
{
    program_options po;
    EXPECT_EQ(0, po.size());
}
