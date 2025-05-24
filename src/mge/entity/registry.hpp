// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/noncopyable.hpp"
#include "mge/entity/dllexport.hpp"
#include "mge/entity/flecs.hpp"

namespace mge::entity {

    /**
     * @brief Entity component system.
     *
     * A system - sometimes called *world* - is a container for entities and
     * components. It is the main entry point for creating and managing
     * entities.
     *
     * Different systems can be used to manage different sets of entities and
     * components, which do not interfere with each other.
     */
    class MGEENTITY_EXPORT registry : public mge::noncopyable
    {
    public:
        /**
         * @brief Create a new empty registry.
         */
        registry();

        /**
         * @brief Destroy the registry.
         */
        ~registry();

        /**
         * @brief Get numnber of entities in the registry.
         *
         * @return number of entities in the registry
         */
        uint32_t entity_count() const;

    private:
        ecs_world_t* m_world; //!< Flecs world instance
        uint32_t     m_index; //!< Index in the registry list
    };

} // namespace mge::entity
