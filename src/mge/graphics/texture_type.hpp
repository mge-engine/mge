// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include <format>
#include <iosfwd>

namespace mge {
    /**
     * Type of texture.
     */
    enum class MGEGRAPHICS_EXPORT texture_type
    {
        TYPE_1D,      //!< 1D texture
        TYPE_2D,      //!< 2D texture
        TYPE_3D,      //!< 3D texture
        TYPE_CUBE_MAP //!< Cube map texture
    };

    /**
     * @brief Output stream operator for texture type.
     *
     * @param os output stream
     * @param t  texture type
     * @return output stream
     */
    MGEGRAPHICS_EXPORT std::ostream& operator<<(std::ostream&       os,
                                                const texture_type& t);
} // namespace mge

/**
 * @brief Formatter implementation for texture type.
 *
 * @tparam C character type
 */
template <typename C>
struct std::formatter<mge::texture_type, C>
    : std::formatter<std::string_view, C>
{
    auto format(const mge::texture_type& t,
                auto& ctx) const -> std::format_context::iterator
    {
        switch (t) {
        case mge::texture_type::TYPE_1D:
            return std::formatter<std::string_view, C>::format(
                std::string_view{"TYPE_1D"},
                ctx);
        case mge::texture_type::TYPE_2D:
            return std::formatter<std::string_view, C>::format(
                std::string_view{"TYPE_2D"},
                ctx);
        case mge::texture_type::TYPE_3D:
            return std::formatter<std::string_view, C>::format(
                std::string_view{"TYPE_3D"},
                ctx);
        case mge::texture_type::TYPE_CUBE_MAP:
            return std::formatter<std::string_view, C>::format(
                std::string_view{"TYPE_CUBE_MAP"},
                ctx);
        }
        return std::format_to(ctx.out(), "INVALID({})", static_cast<int>(t));
    }
};