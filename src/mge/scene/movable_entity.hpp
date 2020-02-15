#pragma once
#include "mge/scene/dllexport.hpp"
#include "mge/entity/entity_registry.hpp"
#include "mge/entity/entity.hpp"
namespace mge {

    template <typename CRP>
    class movable_entity
    {
    private:
        using CRP = base;

        mge::entity _entity() const
        {
            static_cast<const base *>(this)->entity();
        }

        mge::entity_registry& _registry() const
        {
            static_cast<const base *>(this)->registry();
        }

    public:
        movable_entity() = default;
        ~movable_entity() = default;

    };

}
