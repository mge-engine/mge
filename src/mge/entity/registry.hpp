#pragma once
#include "mge/entity/dllexport.hpp"
#include "mge/entity/id_type.hpp"
#include <atomic>

namespace mge::entity {

    class MGEENTITY_EXPORT registry
    {
    public:
        registry() = default;
        ~registry() = default;

    private:
        std::atomic<id_type> m_id_sequence;
    };
} // namespace mge::entity