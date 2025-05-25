// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/noncopyable.hpp"
#include "mge/entity/entity.hpp"
#include "mge/entity/registry.hpp"
#include "mge/scene/dllexport.hpp"

namespace mge::scene {

    /**
     * @brief A world.
     *
     * A world defines a collection of entities which exist and are
     * visualized by scenes.
     */
    class MGESCENE_EXPORT world : public mge::noncopyable
    {
    public:
        /**
         * @brief Create a new empty world.
         */
        world();

        /**
         * @brief Destroy the world.
         */
        ~world();

        inline mge::entity::entity& world_entity() noexcept
        {
            return m_world_entity;
        }

        inline const mge::entity::entity& world_entity() const noexcept
        {
            return m_world_entity;
        }

        inline mge::entity::registry& world_registry() noexcept
        {
            return m_registry;
        }

        inline const mge::entity::registry& world_registry() const noexcept
        {
            return m_registry;
        }

        inline mge::entity::entity& scene_type() noexcept
        {
            return m_scene_type;
        }

    private:
        mge::entity::registry m_registry;
        mge::entity::entity   m_world_entity;  //!< world entity
        mge::entity::entity   m_scene_type;    //!< scene type entity
        mge::entity::entity   m_actor_type;    //!< actor type entity
        mge::entity::entity   m_camera_type;   //!< camera type entity
        mge::entity::entity   m_light_type;    //!< light type entity
        mge::entity::entity   m_geometry_type; //!< geometry type entity
    };

} // namespace mge::scene