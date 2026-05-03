// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/scene/node.hpp"
#include "mge/scene/scene.hpp"
#include "mge/scene/world.hpp"
#include "test/googletest.hpp"

#include <glm/gtc/matrix_transform.hpp>

TEST(scene, construct)
{
    mge::scene s;
    (void)s;
}

TEST(world, scenes_share_entities)
{
    mge::world w;
    auto       a = w.create_scene();
    auto       b = w.create_scene();
    auto       e = a.create_entity("shared_entity");

    EXPECT_EQ(b.world().lookup("shared_entity").id(), e.id());
}

TEST(node, transform_identity_on_default)
{
    mge::scene s;
    auto       n = s.create_node("t");
    EXPECT_EQ(n.transform(), mge::fmat4{1.f});
}

TEST(node, transform_recomputed_on_set_position)
{
    mge::scene s;
    auto       n = s.create_node("t");
    n.set_position({3.f, 0.f, 0.f});
    mge::fmat4 expected =
        glm::translate(mge::fmat4{1.f}, mge::fvec3{3.f, 0.f, 0.f});
    EXPECT_EQ(n.transform(), expected);
}

TEST(node, transform_recomputed_on_set_scale)
{
    mge::scene s;
    auto       n = s.create_node("t");
    n.set_scale({2.f, 2.f, 2.f});
    mge::fmat4 expected =
        glm::scale(mge::fmat4{1.f}, mge::fvec3{2.f, 2.f, 2.f});
    EXPECT_EQ(n.transform(), expected);
}

TEST(node, transform_recomputed_on_set_rotation)
{
    mge::scene s;
    auto       n = s.create_node("t");
    mge::fquat r =
        glm::angleAxis(glm::radians(90.f), mge::fvec3{0.f, 1.f, 0.f});
    n.set_rotation(r);
    mge::fmat4 expected = glm::mat4_cast(r);
    EXPECT_EQ(n.transform(), expected);
}

TEST(node, transform_combined_trs)
{
    mge::scene s;
    auto       n = s.create_node("t");
    mge::fvec3 pos{1.f, 2.f, 3.f};
    mge::fquat rot =
        glm::angleAxis(glm::radians(45.f), mge::fvec3{0.f, 0.f, 1.f});
    mge::fvec3 sc{2.f, 2.f, 2.f};
    n.set_position(pos);
    n.set_rotation(rot);
    n.set_scale(sc);
    mge::fmat4 expected = glm::translate(mge::fmat4{1.f}, pos) *
                          glm::mat4_cast(rot) * glm::scale(mge::fmat4{1.f}, sc);
    EXPECT_EQ(n.transform(), expected);
}
