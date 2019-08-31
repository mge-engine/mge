#pragma once
#include "mge/entity/dllexport.hpp"
#include "mge/entity/entity.hpp"
#include <atomic>
#include <set>
#include <vector>

namespace mge {

    class MGE_ENTITY_EXPORT entity_registry 
    {
    public:
        entity_registry();
        ~entity_registry();

        inline entity create()
        {
            return entity(++m_entity_sequence);
        }

        void destroy(const entity& e);
    private:
        std::atomic<uint64_t> m_entity_sequence;
        std::vector<uint64_t> m_destroyed_entities;
    };
}