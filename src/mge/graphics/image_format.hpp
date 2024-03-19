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
     * @brief Format of an image.
     */
    enum class MGEGRAPHICS_EXPORT image_format : uint8_t
    {
        RGBA_BYTE,  ///< 8-bit per channel
        RGB_BYTE,   ///< 8-bit per channel, no alpha
        RGBA_FLOAT, ///< 32-bit float per channel
        RGB_FLOAT,  ///< 32-bit float per channel, no alpha
        MAX_IMAGE_FORMAT = RGB_FLOAT,
    };

    MGEGRAPHICS_EXPORT std::ostream& operator<<(std::ostream&       os,
                                                const image_format& f);

    /**
     * @brief Binary size of one element of the given format.
     *
     * @param f image format
     * @return size in bytes, 0 if format has no element-based size
     */
    MGEGRAPHICS_EXPORT size_t binary_size(image_format f);

} // namespace mge

template <typename C>
struct std::formatter<mge::image_format, C>
    : std::formatter<std::string_view, C>
{
    auto format(const mge::image_format& f,
                auto& ctx) const -> std::format_context::iterator
    {
        switch (f) {
        case mge::image_format::RGBA_BYTE:
            return std::formatter<std::string_view, C>::format(
                std::string_view{"RGBA_BYTE"},
                ctx);
        case mge::image_format::RGB_BYTE:
            return std::formatter<std::string_view, C>::format(
                std::string_view{"RGB_BYTE"},
                ctx);
        case mge::image_format::RGBA_FLOAT:
            return std::formatter<std::string_view, C>::format(
                std::string_view{"RGBA_FLOAT"},
                ctx);
        case mge::image_format::RGB_FLOAT:
            return std::formatter<std::string_view, C>::format(
                std::string_view{"RGB_FLOAT"},
                ctx);
        }
        return std::format_to(ctx.out(), "INVALID({})", static_cast<int>(f));
    }
};