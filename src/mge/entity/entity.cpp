#include "mge/entity/entity.hpp"
#include "mge/entity/registry.hpp"

namespace mge::entity {
    entity::~entity()
    {
        if (m_id) {
            registry::get(m_id).discard_entity(*this);
            m_id = 0;
        }
    }
} // namespace mge::entity