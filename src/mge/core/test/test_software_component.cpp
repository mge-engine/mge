// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/software_component.hpp"
#include "test/googletest.hpp"

TEST(software_component, mge)
{
    auto mge = mge::software_component::mge();
    EXPECT_EQ("mge", mge->name());
}