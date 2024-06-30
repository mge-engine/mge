// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/entity/dllexport.hpp"
#include "mge/entity/id_type.hpp"
#include <mge/core/format.hpp>

namespace mge::entity {

    /**
     * @brief An entity is an object in the entity-component-system.
     *
     * It is a wrapper around an entity id.
     */
    class MGEENTITY_EXPORT entity
    {
    public:
        /**
         * @brief Default constructor. Creates an invalid entity.
         */
        entity() = default;

        /**
         * @brief Construct an entity with a given id.
         * @param id entity id
         */
        explicit entity(id_type id)
            : m_id(id)
        {}

        /**
         * @brief Move constructor.
         * @param e entity to move from
         */
        entity(entity&& e)
            : m_id(e.m_id)
        {
            e.m_id = 0;
        }

        /**
         * @brief Move assignment.
         * @param e entity to move from
         * @return *this
         */
        entity& operator=(entity&& e)
        {
            m_id = e.m_id;
            e.m_id = 0;
            return *this;
        }

        /**
         * @brief Get the id of the entity.
         * @return entity id
         */
        inline constexpr auto id() const { return m_id; }

        /**
         * @brief Formatting function.
         * Formats the entity as "entity#<id>".
         * @tparam FormatContext format context
         * @param ctx format context
         */
        template <typename FormatContext>
        inline constexpr void format(FormatContext& ctx) const
        {
            std::format_to(ctx.out(), "entity#{}", id());
        }

    private:
        id_type m_id{0};
    };

} // namespace mge::entity
