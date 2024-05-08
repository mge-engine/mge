#pragma once
#include "mge/entity/dllexport.hpp"
#include "mge/entity/entity_id.hpp"
#include <mge/core/format.hpp>

namespace mge::entity {
    class MGENTITY_DLLEXPORT entity
    {
    public:
        entity() = default;

        explicit entity(id_type id)
            : m_id(id)
        {}

        inline constexpr auto id() const { return m_id; }

        template <typename FormatContext>
        inline constexpr void format(FormatContext& ctx) const
        {
            std::format_to(ctx.out(), "entity#{}", id());
        }

    private:
        const id_type m_id{0};
    };
}
}