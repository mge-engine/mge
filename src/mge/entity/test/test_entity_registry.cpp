// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "mge/entity/entity_registry.hpp"

using namespace mge;
using namespace testing;

TEST(entity_registry, create_destroy)
{
    entity_registry registry;
}

TEST(entity_registry, current_version)
{
    entity_registry registry;
    auto e = registry.create();
    auto v_alive = registry.current(e);
    registry.destroy(e);
    auto v_dead = registry.current(e);
    EXPECT_NE(v_alive, v_dead);
}

TEST(entity_registry, entity_to_integer)
{
    entity_registry registry;
    auto e = registry.create();
    mge::to_integer(e);
}

TEST(entity_registry, entity_valid)
{
    entity_registry registry;
    auto e = registry.create();
    EXPECT_TRUE(registry.valid(e));
    registry.destroy(e);
    EXPECT_FALSE(registry.valid(e));
}