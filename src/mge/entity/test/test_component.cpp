// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/tagged.hpp"
#include "mge/entity/entity_registry.hpp"
#include "mge/math/vec3.hpp"
#include "test/googletest.hpp"

using namespace mge;

TEST(component, tagged_values)
{
    entity_registry registry;
    auto            e = registry.create();

    struct position_tag
    {};
    struct look_at_tag
    {};

    using position = mge::tagged<mge::fvec3, position_tag>;
    using look_at  = mge::tagged<mge::fvec3, look_at_tag>;

    registry.assign<position>(e, 1.0f, 1.0f, 1.0f);
    registry.assign<look_at>(e, fvec3(0.0f, 0.0f, 1.0f));

    fvec3 pos = registry.get<position>(e);
    EXPECT_EQ(pos, fvec3(1.0f, 1.0f, 1.0f));
}
