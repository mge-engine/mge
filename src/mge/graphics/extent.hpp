// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include <cstdint>
#include <iosfwd>

namespace mge {
    /**
     * @brief Extent defined by width and height.
     */
    class MGEGRAPHICS_EXPORT extent
    {
    public:
        extent()
            : width(0u)
            , height(0u)
        {}

        extent(uint32_t width_, uint32_t height_)
            : width(width_)
            , height(height_)
        {}

        extent(const extent&) = default;
        extent(extent&&) = default;

        extent& operator=(const extent&) = default;
        extent& operator=(extent&&) = default;

        bool operator==(const extent& e) const
        {
            return width == e.width && height == e.height;
        }

        bool operator!=(const extent& e) const
        {
            return width != e.width || height != e.height;
        }

        uint32_t width;
        uint32_t height;
    };

    MGEGRAPHICS_EXPORT std::ostream& operator<<(std::ostream& os,
                                                const extent& d);
} // namespace mge
