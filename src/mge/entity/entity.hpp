// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/entity/dllexport.hpp"
#include "mge/entity/entity_fwd.hpp"
#include "mge/entity/flecs.hpp"

namespace mge::entity {

    /**
     * @brief Concept that checks if a type has an entity() method returning
     * <tt>mge::entity::entity&</tt>
     */
    template <typename T>
    concept has_entity = requires(T& x) {
        { x.entity() } -> std::same_as<mge::entity::entity&>;
    };

    class MGEENTITY_EXPORT entity : public flecs::entity
    {
    public:
        static decltype(flecs::ChildOf) child_of;
        static decltype(flecs::IsA)     is_a;

        entity() = default;
        explicit entity(mge::entity::registry& registry);

        entity(mge::entity::registry& registry, const char* name);

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