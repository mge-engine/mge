// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/entity/registry.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/stdexceptions.hpp"
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
            registry_id_type index = m_registry_index_hint;
            uint32_t         retry = 0;

            auto start_index = index;
            while (retry < 3) {
                registry* expected = nullptr;
                if (m_registries[index].compare_exchange_strong(expected, r)) {
                    // not exact, only a scan start hint
                    m_registry_index_hint = index + 1;
                    return index + 1;
                }
                ++index;
                if (index == start_index) {
                    ++retry;
                }
            }

            MGE_THROW(mge::runtime_exception)
                << "Cannot register entity registry, all slots are used";
        }

        void unregister_registry(registry_id_type id, registry* r)
        {
            if (m_registries[id - 1].compare_exchange_strong(r, nullptr)) {
                return;
            } else {
                MGE_THROW(mge::runtime_exception)
                    << "Cannot unregister entity registry, registry not found";
            }
        }

        registry* get(registry_id_type id)
        {
            if (id == 0 || id > MAX_REGISTRIES) {
                return nullptr;
            }
            return m_registries[id - 1].load();
        }

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
        registry_registry& rr = *s_registry_registry;
        m_registry_id = rr.register_registry(this);
    }

    registry::~registry()
    {
        registry_registry& rr = *s_registry_registry;
        rr.unregister_registry(m_registry_id, this);
    }

    registry& registry::get(registry_id_type id)
    {
        registry_registry& rr = *s_registry_registry;
        registry*          r = rr.get(id);
        if (r) {
            return *r;
        } else {
            MGE_THROW(mge::runtime_exception) << "Entity registry not found";
        }
    }

    registry& registry::get(id_type id)
    {
        registry_id_type rid = static_cast<registry_id_type>(id >> 48);
        return get(rid);
    }

} // namespace mge::entity
