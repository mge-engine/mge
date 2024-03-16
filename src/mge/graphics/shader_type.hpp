// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include <cstdint>
#include <format>
#include <iosfwd>

namespace mge {

    /**
     * Type of shader.
     */
    enum class MGEGRAPHICS_EXPORT shader_type : uint8_t
    {
        VERTEX = 0,                 //!< vertex
        FRAGMENT = 1,               //!< fragment (pixel)
        COMPUTE = 2,                //!< compute
        TESSELATION_CONTROL = 3,    //!< tesselation control (hull)
        TESSELATION_EVALUATION = 4, //!< tesselation evaluation (domain)
        GEOMETRY = 5,               //!< geometry
        MAX_SHADER_TYPE = GEOMETRY
    };
    MGEGRAPHICS_EXPORT std::ostream& operator<<(std::ostream&      os,
                                                const shader_type& t);
} // namespace mge

template <typename C>
struct std::formatter<mge::shader_type, C> : std::formatter<std::string_view, C>
{
    auto format(const mge::shader_type& t,
                auto& ctx) const -> std::format_context::iterator
    {
        switch (t) {
        case mge::shader_type::VERTEX:
            return std::formatter<std::string_view, C>::format(
                std::string_view{"VERTEX"},
                ctx);
        case mge::shader_type::FRAGMENT:
            return std::formatter<std::string_view, C>::format(
                std::string_view{"FRAGMENT"},
                ctx);
        case mge::shader_type::COMPUTE:
            return std::formatter<std::string_view, C>::format(
                std::string_view{"COMPUTE"},
                ctx);

        case mge::shader_type::TESSELATION_CONTROL:
            return std::formatter<std::string_view, C>::format(
                std::string_view{"TESSELATION_CONTROL"},
                ctx);

        case mge::shader_type::TESSELATION_EVALUATION:
            return std::formatter<std::string_view, C>::format(
                std::string_view{"TESSELATION_EVALUATION"},
                ctx);

        case mge::shader_type::GEOMETRY:
            return std::formatter<std::string_view, C>::format(
                std::string_view{"GEOMETRY"},
                ctx);

        default:
            return std::format_to(ctx.out(), "INVALID({})", (int)t);
        }
    }
};
