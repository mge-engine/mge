// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"

#include "mge/entity/entity.hpp"
#include "mge/entity/registry.hpp"

TEST(registry, create)
{
    mge::entity::registry s;
}

TEST(registry, entity_count_initially_zero)
{
    mge::entity::registry s;
    EXPECT_EQ(s.entity_count(), 0);
}

TEST(registry, create_entity)
{
    mge::entity::registry s;
    mge::entity::entity   e(s);
    EXPECT_EQ(s.entity_count(), 1);
}
