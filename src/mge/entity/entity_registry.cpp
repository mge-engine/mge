#include "mge/entity/entity_registry.hpp"

namespace mge {
    
    entity_registry::entity_registry()
        :m_entity_sequence(0)
    {}

    entity_registry::~entity_registry()
    {}

    void 
    entity_registry::destroy(const entity& e)
    {
        m_destroyed_entities.emplace_back(e.id());
    }
}