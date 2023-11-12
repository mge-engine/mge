// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
/** @file */
#pragma once
#include "mge/graphics/dllexport.hpp"
#include <cstdint>
#include <iosfwd>
#include <format>

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
}

namespace std {


    template <>
    struct std::formatter<mge::shader_type> : std::formatter<std::string_view> {
        auto format(mge::shader_type t, std::format_context& ctx) -> std::format_context::iterator {
            std::string_view name;
            switch (t) {
                case mge::shader_type::VERTEX:
                    name = "VERTEX";
                    break;
                case mge::shader_type::FRAGMENT:
                    name = "FRAGMENT";
                    break;
                case mge::shader_type::COMPUTE:
                    name = "COMPUTE";
                    break;
                case mge::shader_type::TESSELATION_CONTROL:
                    name = "TESSELATION_CONTROL";
                    break;
                case mge::shader_type::TESSELATION_EVALUATION:
                    name = "TESSELATION_EVALUATION";
                    break;
                case mge::shader_type::GEOMETRY:
                    name = "GEOMETRY";
                    break;
                default:
                    return std::format_to(ctx.out(), "INVALID({})", (int)t);
                    break;
            }
            return std::formatter<std::string_view>::format(name, ctx);
        }
    };
}
