// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/entity/dllexport.hpp"
#include "mge/entity/id_type.hpp"
#include <mge/core/format.hpp>

namespace mge::entity {
    class MGEENTITY_EXPORT entity
    {
    public:
        entity() = default;

        explicit entity(id_type id)
            : m_id(id)
        {}

        entity(entity&& e)
            : m_id(e.m_id)
        {
            e.m_id = 0;
        }

        entity& operator=(entity&& e)
        {
            m_id = e.m_id;
            e.m_id = 0;
            return *this;
        }

        inline constexpr auto id() const { return m_id; }

        template <typename FormatContext>
        inline constexpr void format(FormatContext& ctx) const
        {
            std::format_to(ctx.out(), "entity#{}", id());
        }

    private:
        id_type m_id{0};
    };

} // namespace mge::entity
