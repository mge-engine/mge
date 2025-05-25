// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"

#include "mge/scene/scene.hpp"

TEST(scene, create)
{

    mge::scene::world w;
    mge::scene::scene s(w);
    EXPECT_TRUE(s.scene_entity().is_valid());
    std::cout << "World created with entity: " << w.world_entity().path()
              << std::endl;
    std::cout << "Scene created with entity: " << s.scene_entity().path()
              << std::endl;
}