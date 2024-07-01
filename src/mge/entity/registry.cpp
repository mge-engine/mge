// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/entity/registry.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/trace.hpp"
#include <array>
#include <limits>

namespace mge {
    MGE_DEFINE_TRACE(ENTITY);
}
namespace mge::entity {

    class registry_registry
    {
    public:
        registry_registry() = default;
        ~registry_registry() = default;

        registry_id_type register_registry(registry* r)
        {
            registry_id_type id = 0;
            return id;
        }

        void unregister_registry(registry_id_type id, registry* r) {}

    private:
        static const size_t MAX_REGISTRIES =
            std::numeric_limits<registry_id_type>::max();
        std::array<std::atomic<registry*>, MAX_REGISTRIES> m_registries;
        registry_id_type m_registry_index_hint;
    };

    static singleton<registry_registry> s_registry_registry;

    registry::registry()
        : m_id_sequence(1)
        , m_registry_id(0)
    {
        registry_registry& rr = s_registry_registry.instance();
        m_registry_id = rr.register_registry(this);
    }

    registry::~registry()
    {
        registry_registry& rr = s_registry_registry.instance();
        rr.unregister_registry(m_registry_id, this);
    }

    registry& registry::get(registry_id_type id) {}

} // namespace mge::entity
