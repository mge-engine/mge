// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.

#include "mge/entity/registry.hpp"
#include "test/googletest.hpp"

TEST(registry, create)
{
    mge::entity::registry r;
    EXPECT_GE(r.id(), 1);
}