#pragma once
#include "mge/entity/entity_registry.hpp"
#include "mge/entity/entity.hpp"
namespace mge {

    class entity_class
    {
    protected:
        explicit entity_class(entity_registry& registry)
            :m_registry(registry)
            ,m_entity(0)
        {

        }
    private:
        entity_registry* m_registry;
        entity           m_entity;
    };

}
