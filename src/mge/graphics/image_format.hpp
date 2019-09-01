#pragma once
#include "mge/graphics/dllexport.hpp"
#include <cstdint>
#include <iosfwd>

namespace mge {

    enum class MGE_GRAPHICS_EXPORT image_format : std::uint16_t
    {
        UNKNOWN = 0,
        ABGR8 = 1,
        A8 = 2,
        ARGB8 = 3,
        BGR8 = 4,
        RGB8 = 5,
        RGB16F = 6,
        RGB32F = 7,
        RGBA8 = 8
    };

    MGE_GRAPHICS_EXPORT std::ostream& operator <<(std::ostream& os, const image_format& f);

    MGE_GRAPHICS_EXPORT size_t pixel_size(const image_format& f);

}
