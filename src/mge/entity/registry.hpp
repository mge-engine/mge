// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/entity/dllexport.hpp"
#include "mge/entity/entity.hpp"
#include "mge/entity/id_type.hpp"

#include <atomic>
#include <set>

namespace mge::entity {

    /** @brief An entity and component registry.
     *
     * Each entity has a registry that manages the entity and its components.
     */
    class MGEENTITY_EXPORT registry
    {
    public:
        /**
         * Construct a registry.
         *
         * @throws runtime_
         */
        registry();

        /**
         * Destruct a registry.
         */
        ~registry();

        /**
         * Get id of registry.
         * @return registry id
         */
        inline constexpr registry_id_type id() const noexcept
        {
            return m_registry_id;
        }

        /**
         * Get the registry.
         * @param id registry id
         */
        static registry& get(registry_id_type id);

        /**
         * Get the registr by entity id.
         * @param id entity id
         */
        static registry& get(id_type id);

        /**
         * Create a new entity id.
         * @return new entity id
         */
        entity create_entity();

        /**
         * Discard an entity id.
         * @param e entity to discard
         */
        void discard_entity(entity& e);

    private:
        std::atomic<id_type> m_id_sequence;
        registry_id_type     m_registry_id;

        std::set<id_type> m_discarded_entities;
    };
} // namespace mge::entity