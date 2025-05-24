// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/entity/registry.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"

#include <array>
#include <atomic>

namespace mge {
    MGE_DEFINE_TRACE(ENTITY);
}

namespace mge::entity {

    class registry_list
    {
    public:
        registry_list() = default;
        ~registry_list() = default;

        registry* get(size_t index) const
        {
            return m_registries[index].load();
        }

        uint32_t add_registry(registry* reg)
        {
            bool anything_tried = true;
            while (anything_tried) {
                anything_tried = false;
                for (size_t i = 0; i < m_registries.size(); ++i) {
                    if (m_registries[i].load() == nullptr) {
                        anything_tried = true;
                        registry* expected = nullptr;
                        if (m_registries[i].compare_exchange_strong(expected,
                                                                    reg)) {
                            return static_cast<uint32_t>(i);
                        }
                    }
                }
            }
            MGE_THROW(mge::runtime_exception) << "No more registries available";
        }

    private:
        std::array<std::atomic<registry*>, 127> m_registries;
    };

    mge::singleton<registry_list> s_registry_list;

    registry::registry()
        : m_world(ecs_init())
        , m_index(s_registry_list->add_registry(this))
    {
        if (m_world == nullptr) {
            MGE_THROW(mge::runtime_exception)
                << "Failed to initialize flecs world";
        }
    }

    registry::~registry()
    {
        if (m_world != nullptr) {
            if (ecs_fini(m_world)) {
                MGE_ERROR_TRACE(ENTITY) << "Failed to finalize flecs world";
            }
            m_world = nullptr;
        }
    }

    uint32_t registry::entity_count() const
    {
        auto entities = ecs_get_entities(m_world);
        return entities.count;
    }

} // namespace mge::entity