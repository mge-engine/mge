// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/entity/entity.hpp"
#include "mge/core/atomic.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/entity/registry.hpp"

namespace mge::entity {
    entity::entity()
        : m_entity(0)
        , m_flags(0)
    {}

    entity::entity(const registry& r)
    {
        m_entity = ecs_new(r.m_world);
        if (m_entity == 0) {
            MGE_THROW(mge::runtime_exception)
                << "Failed to create entity in registry";
        }
        m_flags.index = r.m_index;
        m_flags.reserved0 = 0;
    }

    entity::~entity()
    {
        if (m_entity != 0) {
            ecs_delete(registry::get(m_flags.index).m_world, m_entity);
        }
    }

    entity::entity(entity&& other)
        : m_entity(other.m_entity)
        , m_flags(other.m_flags)
    {
        other.m_entity = 0; // Prevent deletion in the destructor
        other.m_flags.index = 0;
        other.m_flags.reserved0 = 0;
    }

    entity& entity::operator=(entity&& other)
    {
        if (this != &other) {
            if (m_entity != 0) {
                ecs_delete(registry::get(m_flags.index).m_world, m_entity);
            }
            m_entity = other.m_entity;
            m_flags = other.m_flags;
            other.m_entity = 0; // Prevent deletion in the destructor
            other.m_flags.index = 0;
            other.m_flags.reserved0 = 0;
        }
        return *this;
    }

} // namespace mge::entity