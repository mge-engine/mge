// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/entity/dllexport.hpp"
#include "mge/entity/entity_fwd.hpp"
#include "mge/entity/flecs.hpp"

#include <atomic>

namespace mge::entity {

    /**
     * @brief Entity class.
     *
     * This class is used to represent an entity.
     */
    class MGEENTITY_EXPORT entity
    {
    public:
        entity();
        entity(const registry& r);
        ~entity();

        entity(const entity&) = delete; //!< Copy constructor is deleted
        entity(entity&&);

        entity& operator=(const entity&) = delete;
        entity& operator=(entity&&);

    private:
        ecs_entity_t m_entity; //!< Flecs entity handle

        struct flags
        {
            uint32_t index : 7; //!< Index in the registry list
            uint32_t reserved0 : 25;
        };
        flags m_flags;
    };
} // namespace mge::entity