// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/core/format.hpp"
#include "mge/math/dllexport.hpp"
#include "mge/math/glm.hpp"

#include <concepts>
#include <iostream>
#include <type_traits>

namespace mge {

    using fquat = glm::quat;
    using dquat = glm::dquat;

    template <typename T>
    concept quat_type = std::same_as<T, fquat> || std::same_as<T, dquat>;

    // Euler angle extraction
    using glm::eulerAngles;
    using glm::pitch;
    using glm::roll;
    using glm::yaw;

    // Matrix conversions
    using glm::mat3_cast;
    using glm::mat4_cast;
    using glm::quat_cast;

    // Look-at constructors
    using glm::quatLookAt;
    using glm::quatLookAtLH;
    using glm::quatLookAtRH;

    // Interpolation
    using glm::fastMix;
    using glm::intermediate;
    using glm::lerp;
    using glm::mix;
    using glm::shortMix;
    using glm::slerp;
    using glm::squad;

    // Operations
    using glm::conjugate;
    using glm::inverse;
    using glm::length2;
    using glm::quat_identity;
    using glm::rotate;
    using glm::rotation;

} // namespace mge

template <> struct fmt::formatter<mge::fquat> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::fquat& q, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(), "({}, {}, {}, {})", q.w, q.x, q.y, q.z);
        return ctx.out();
    }
};
template <> struct fmt::formatter<mge::dquat> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::dquat& q, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(), "({}, {}, {}, {})", q.w, q.x, q.y, q.z);
        return ctx.out();
    }
};

namespace mge {

    inline std::ostream& operator<<(std::ostream& os, const fquat& q)
    {
        fmt::print(os, "{}", q);
        return os;
    }
    inline std::ostream& operator<<(std::ostream& os, const dquat& q)
    {
        fmt::print(os, "{}", q);
        return os;
    }

} // namespace mge
