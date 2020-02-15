#pragma once
#include "mge/scene/dllexport.hpp"
#include "mge/entity/entity_registry.hpp"
#include "mge/entity/entity.hpp"
#include "mge/entity/component.hpp"
#include "mge/math/mat4.hpp"
#include "mge/math/vec3.hpp"
#include "mge/math/quat.hpp"

namespace mge {

    template <typename B>
    class movable_entity
    {
    private:
        inline mge::entity_registry& registry() const
        {
            const B * base = static_cast<const B*>(this);
            return base->registry();
        }

        inline mge::entity entity() const
        {
            const B * base = static_cast<const B*>(this);
            return base->entity();
        }

    public:
        movable_entity() = default;
        ~movable_entity() = default;

        MGE_ENTITY_COMPONENT(fvec3, position)
        MGE_ENTITY_COMPONENT(fquat, orientation)
        MGE_ENTITY_COMPONENT(fvec3, scale)
        MGE_ENTITY_COMPONENT(fmat4, transform)
    };

}
