// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/format.hpp"
#include "mge/graphics/dllexport.hpp"
#include <cstdint>

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

        constexpr bool operator==(const extent& e) const
        {
            return width == e.width && height == e.height;
        }

        constexpr bool operator!=(const extent& e) const
        {
            return width != e.width || height != e.height;
        }

        size_t area() const noexcept
        {
            return static_cast<size_t>(width) * static_cast<size_t>(height);
        }

        inline void format(std::format_context& ctx) const
        {
            std::format_to(ctx.out(),
                           "extent{{width={}, height={}}}",
                           width,
                           height);
        }

        uint32_t width;
        uint32_t height;
    };

} // namespace mge
