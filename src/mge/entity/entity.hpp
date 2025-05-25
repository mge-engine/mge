// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/entity/dllexport.hpp"
#include "mge/entity/entity_fwd.hpp"
#include "mge/entity/flecs.hpp"

namespace mge::entity {

    class MGEENTITY_EXPORT entity : public flecs::entity
    {
    public:
        entity() = default;
        explicit entity(mge::entity::registry& registry);
        ~entity() = default;

        entity(const entity&);
        entity& operator=(const entity&);

        entity(entity&&) noexcept;
        entity& operator=(entity&&) noexcept;

        void destroy() const
        {
            if (is_valid()) {
                flecs::entity::destruct();
            }
        }
    };

} // namespace mge::entity