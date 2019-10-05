#pragma once
#include "mge/entity/dllexport.hpp"
#include "mge/entity/entt.hpp"
#include <cstdint>

namespace mge {
    using entity = entt::entity;
    constexpr auto nil_entity = entt::null;

    inline constexpr auto to_integer(const entity id) noexcept {
        return std::underlying_type_t<entity>(id);\
    }

}