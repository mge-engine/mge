// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/entity/dllexport.hpp"
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
        ~entity() = default;

        entity(const entity&) = delete;
        entity(entity&&) = delete;

        entity& operator=(const entity&) = delete;
        entity& operator=(entity&&) = delete;

    private:
        ecs_entity_t m_entity; //!< Flecs entity handle

        struct flags
        {
            uint32_t index : 7; //!< Index in the registry list
            uint32_t reserved0 : 25;
        };
        flags                 m_flags;
        std::atomic<uint32_t> m_refcount; //!< Reference count for the entity
    };
} // namespace mge::entity