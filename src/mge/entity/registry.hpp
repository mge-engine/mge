// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/entity/dllexport.hpp"
#include "mge/entity/entity.hpp"
#include "mge/entity/id_type.hpp"
#include <atomic>

namespace mge::entity {

    class MGEENTITY_EXPORT registry
    {
    public:
        registry();

        ~registry();

        inline constexpr registry_id_type id() const noexcept
        {
            return m_registry_id;
        }

    private:
        std::atomic<id_type>                 m_id_sequence;
        registry_id_type                     m_registry_id;
        static std::atomic<registry_id_type> s_registry_id_sequence;
    };
} // namespace mge::entity