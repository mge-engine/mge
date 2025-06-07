// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"

#include "mge/entity/entity.hpp"
#include "mge/entity/registry.hpp"

TEST(entity, create)
{
    mge::entity::registry s;
    mge::entity::entity   e(s);
    EXPECT_TRUE(e.is_valid());
}

TEST(entity, create_destroy)
{
    mge::entity::registry s;
    mge::entity::entity   e(s);
    EXPECT_TRUE(e.is_valid());
    e.destroy();
    EXPECT_FALSE(e.is_valid());
}

TEST(entity, move_construct)
{
    mge::entity::registry s;
    mge::entity::entity   e(s);
    mge::entity::entity   e2(std::move(e));
    EXPECT_TRUE(e2.is_valid());
    EXPECT_FALSE(e.is_valid()); // e should be invalid after move
}

TEST(entity, move_assign)
{
    mge::entity::registry s;
    mge::entity::entity   e(s);
    EXPECT_TRUE(e.is_valid());
    mge::entity::entity e2(s);
    e2 = std::move(e);
    EXPECT_TRUE(e2.is_valid());
    EXPECT_FALSE(e.is_valid()); // e should be invalid after move
}

TEST(entity, sizeof)
{
    EXPECT_EQ(sizeof(mge::entity::entity), sizeof(flecs::entity));
    EXPECT_EQ(sizeof(mge::entity::registry), sizeof(flecs::world));
    EXPECT_EQ(16u,
              sizeof(mge::entity::entity)); // 2 * 8 bytes for world_ and id_
}