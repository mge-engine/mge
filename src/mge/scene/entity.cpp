// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/scene/entity.hpp"

namespace mge {

    entity::entity(flecs::entity e)
        : m_entity(e)
    {}

    flecs::entity_t entity::id() const noexcept
    {
        return m_entity.id();
    }

    bool entity::valid() const noexcept
    {
        return m_entity.is_valid();
    }

    std::string_view entity::name() const noexcept
    {
        return std::string_view(m_entity.name());
    }

} // namespace mge
