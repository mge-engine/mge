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

TEST(registry, create_destroy_entity)
{
    mge::entity::registry s;
    mge::entity::entity   e(s);
    e.destroy();
}
