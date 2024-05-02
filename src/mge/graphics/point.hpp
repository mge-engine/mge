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
     * A point.
     */
    class MGEGRAPHICS_EXPORT point
    {
    public:
        point()
            : x(0)
            , y(0)
        {}

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

        inline void format(std::format_context& ctx) const
        {
            std::format_to(ctx.out(), "point{{x={}, y={}}}", x, y);
        }

        uint32_t x; //!< x coordinate
        uint32_t y; //!< y coordinate
    };

} // namespace mge
