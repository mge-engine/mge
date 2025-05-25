// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/entity/entity.hpp"
#include "mge/entity/registry.hpp"

namespace mge::entity {
    entity::entity(mge::entity::registry& registry)
        : flecs::entity(registry)
    {}

    entity::entity(const entity& other)
        : flecs::entity(other)
    {}

    entity& entity::operator=(const entity& other)
    {
        if (this != &other) {
            flecs::entity::operator=(other);
        }
        return *this;
    }

    entity::entity(entity&& other) noexcept
        : flecs::entity(std::move(other))
    {
        other = entity();
    }

    entity& entity::operator=(entity&& other) noexcept
    {
        if (this != &other) {
            flecs::entity::operator=(std::move(other));
            other = entity();
        }
        return *this;
    }
} // namespace mge::entity