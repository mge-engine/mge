// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/format.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/extent.hpp"
#include "mge/graphics/point.hpp"
#include "mge/graphics/rectangle.hpp"

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
     * The viewport is defined by the top left corner and the width and
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
         * @param x_         x coordinate of top left corner
         * @param y_         y coordinate of top left corner
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

        /**
         * @brief Set the viewport rectangle.
         *
         * @param x_ left x coordinate
         * @param y_ top y coordinate
         * @param width_ viewport width
         * @param height_ viewport height
         */
        void set_rect(float x_, float y_, float width_, float height_)
        {
            x = x_;
            y = y_;
            width = width_;
            height = height_;
        }

        /**
         * @brief Set the viewport rectangle from an extent.
         *
         * Sets origin to (0, 0) and size from the extent.
         *
         * @param ext extent defining width and height
         */
        void set_rect(const mge::extent& ext)
        {
            x = 0.0f;
            y = 0.0f;
            width = static_cast<float>(ext.width);
            height = static_cast<float>(ext.height);
        }

        /**
         * @brief Set the viewport rectangle from a rectangle.
         *
         * @param rect source rectangle
         */
        void set_rect(const mge::rectangle& rect)
        {
            x = static_cast<float>(rect.left);
            y = static_cast<float>(rect.top);
            width = static_cast<float>(rect.width());
            height = static_cast<float>(rect.height());
        }

        /**
         * Pointer to beginning of data.
         */
        inline const float* data() const
        {
            return (float*)&x;
        }

        /**
         * Lower left corner as point.
         * @param window_height height of window of viewport
         * @return lower left corner
         */
        mge::point lower_left(uint32_t window_height) const
        {
            return mge::point(static_cast<uint32_t>(x),
                              window_height -
                                  static_cast<uint32_t>(y + height));
        }

        float x{0.0f};
        float y{0.0f};
        float width{0.0f};
        float height{0.0f};
        float min_depth{0.0f};
        float max_depth{0.0f};
    };
} // namespace mge

template <>
struct fmt::formatter<mge::viewport> : public fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::viewport& vp, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(),
                       "viewport{{x={}, y={}, width={}, height={}, "
                       "min_depth={}, max_depth={}}}",
                       vp.x,
                       vp.y,
                       vp.width,
                       vp.height,
                       vp.min_depth,
                       vp.max_depth);

        return ctx.out();
    }
};

namespace mge {
    inline std::ostream& operator<<(std::ostream& os, const viewport& vp)
    {
        fmt::print(os, "{}", vp);
        return os;
    }
} // namespace mge
