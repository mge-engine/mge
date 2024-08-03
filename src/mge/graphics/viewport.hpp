// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/format.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/extent.hpp"
#include "mge/graphics/point.hpp"
#include <cstdint>

namespace mge {
#ifdef BUILD_GRAPHICS
    MGE_USE_TRACE(GRAPHICS);
#else
    MGE_USE_IMPORTED_TRACE(GRAPHICS);
#endif

    /**
     * A viewport defines the transformation to the frame buffer.
     *
     * The viewport is defined by the lower left corner and the width and
     * height, as well as the depth range.
     */
    class MGEGRAPHICS_EXPORT viewport
    {
    public:
        /**
         * Construct an empty viewport.
         */
        viewport() {}

        /**
         * Construct a viewport.
         * @param x_         x coordinate of lower left corner
         * @param y_         y coordinate of lower left corner
         * @param width_     width of viewport
         * @param height_    height of viewport
         * @param min_depth_ minimum depth value
         * @param max_depth_ maximum depth value
         */
        viewport(float x_,
                 float y_,
                 float width_,
                 float height_,
                 float min_depth_,
                 float max_depth_)
            : x(x_)
            , y(y_)
            , width(width_)
            , height(height_)
            , min_depth(min_depth_)
            , max_depth(max_depth_)
        {}

        ~viewport() = default;

        viewport(const viewport&) = default;
        viewport(viewport&&) = default;
        viewport& operator=(const viewport&) = default;
        viewport& operator=(viewport&&) = default;

        inline void format(std::format_context& ctx) const
        {
            std::format_to(ctx.out(),
                           "viewport{{x={}, y={}, width={}, height={}, "
                           "min_depth={}, max_depth={}}}",
                           x,
                           y,
                           width,
                           height,
                           min_depth,
                           max_depth);
        }

        /**
         * Pointer to beginning of data.
         */
        inline const float* data() const { return (float*)&x; }

        /**
         * Lower left corner as point.
         * @return lower left corner
         */
        mge::point lower_left() const
        {
            return mge::point(static_cast<uint32_t>(x),
                              static_cast<uint32_t>(y));
        }

        float x{0.0f};
        float y{0.0f};
        float width{0.0f};
        float height{0.0f};
        float min_depth{0.0f};
        float max_depth{0.0f};
    };
} // namespace mge
