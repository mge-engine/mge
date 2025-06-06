// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"

#include "mge/scene/camera.hpp"
#include "mge/scene/scene.hpp"

TEST(camera, create)
{
    mge::scene::world  w;
    mge::scene::camera c(w);
    EXPECT_TRUE(c.camera_entity().is_valid());
}

TEST(camera, set_properties)
{
    mge::scene::world  w;
    mge::scene::camera c(w);
    c.set_look_at(mge::fvec3(1.0f, 2.0f, 3.0f));
    c.set_position(mge::fvec3(0.0f, 4.0f, 0.0f));
    EXPECT_EQ(c.look_at(), mge::fvec3(1.0f, 2.0f, 3.0f));
    EXPECT_EQ(c.position(), mge::fvec3(0.0f, 4.0f, 0.0f));
}

TEST(camera, copy_points_to_same)
{
    mge::scene::world  w;
    mge::scene::camera c1(w);
    c1.set_look_at(mge::fvec3(1.0f, 2.0f, 3.0f));
    c1.set_position(mge::fvec3(0.0f, 4.0f, 0.0f));

    mge::scene::camera c2(c1);
    EXPECT_EQ(c2.look_at(), mge::fvec3(1.0f, 2.0f, 3.0f));
    EXPECT_EQ(c2.position(), mge::fvec3(0.0f, 4.0f, 0.0f));
    EXPECT_EQ(c2.camera_entity(), c1.camera_entity());
}