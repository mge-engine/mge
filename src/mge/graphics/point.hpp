// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/format.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/dllexport.hpp"
#include <cstdint>

namespace mge {

#ifdef BUILD_GRAPHICS
    MGE_USE_TRACE(GRAPHICS);
#else
    MGE_USE_IMPORTED_TRACE(GRAPHICS);
#endif

    /**
     * @brief A point in 2D space with integer coordinates.
     */
    class MGEGRAPHICS_EXPORT point
    {
    public:
        point() {}

        /**
         * @brief Create point.
         * @param x_ x coordinate
         * @param y_ y coordinate
         */
        point(uint32_t x_, uint32_t y_)
            : x(x_)
            , y(y_)
        {}

        ~point() = default;

        point(const point&) = default;
        point(point&&) = default;
        point& operator=(const point&) = default;
        point& operator=(point&&) = default;

        uint32_t x{0}; //!< x coordinate
        uint32_t y{0}; //!< y coordinate
    };

} // namespace mge

template <>
struct fmt::formatter<mge::point> : public fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::point& p, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(), "point{{x={}, y={}}}", p.x, p.y);
        return ctx.out();
    }
};

namespace mge {
    inline std::ostream& operator<<(std::ostream& os, const point& p)
    {
        fmt::print(os, "{}", p);
        return os;
    }
} // namespace mge