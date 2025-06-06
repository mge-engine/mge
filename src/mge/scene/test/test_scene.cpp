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
}
