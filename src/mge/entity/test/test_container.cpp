// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"

#include "mge/entity/container.hpp"
#include "mge/entity/registry.hpp"

TEST(container, create)
{
    mge::entity::registry  s;
    mge::entity::container c(s);
    EXPECT_TRUE(c.is_valid());
}