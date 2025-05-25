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

    private:
        friend class scene;

        mge::entity::registry m_registry;
        mge::entity::entity   m_world_entity; //!< The world entity
    };

} // namespace mge::scene