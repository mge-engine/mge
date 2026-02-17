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

        /**
         * @brief Compute the area of the extent.
         *
         * @return area in pixels (width * height)
         */
        size_t area() const noexcept
        {
            return static_cast<size_t>(width) * static_cast<size_t>(height);
        }

        uint32_t width;
        uint32_t height;
    };

} // namespace mge

template <>
struct fmt::formatter<mge::extent> : public fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::extent& e, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(),
                       "extent{{width={}, height={}}}",
                       e.width,
                       e.height);
        return ctx.out();
    }
};

namespace mge {
    inline std::ostream& operator<<(std::ostream& os, const mge::extent& e)
    {
        fmt::print(os, "{}", e);
        return os;
    }
} // namespace mge
