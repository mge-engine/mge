// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/entity/world.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"

namespace mge {
    MGE_DEFINE_TRACE(ENTITY);
}

namespace mge::entity {

    world::world()
        : m_world(ecs_init())
    {
        if (m_world == nullptr) {
            MGE_THROW(mge::runtime_exception)
                << "Failed to initialize flecs world";
        }
    }

    world::~world()
    {
        if (m_world != nullptr) {
            if (ecs_fini(m_world)) {
                MGE_ERROR_TRACE(ENTITY) << "Failed to finalize flecs world";
            }
            m_world = nullptr;
        }
    }

} // namespace mge::entity