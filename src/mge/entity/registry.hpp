// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/entity/dllexport.hpp"
#include "mge/entity/entity_fwd.hpp"
#include "mge/entity/id_type.hpp"
#include <atomic>

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

        ~registry();

        inline constexpr registry_id_type id() const noexcept
        {
            return m_registry_id;
        }

        static registry& get(registry_id_type id);
        static registry& get(id_type id);

    private:
        std::atomic<id_type> m_id_sequence;
        registry_id_type     m_registry_id;
    };
} // namespace mge::entity