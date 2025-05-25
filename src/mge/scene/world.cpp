// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "world.hpp"

namespace mge::scene {

    world::world()
    {
        m_world_entity = mge::entity::entity(m_registry);
    }

    world::~world()
    {
        m_world_entity.destroy();
    }

} // namespace mge::scene
