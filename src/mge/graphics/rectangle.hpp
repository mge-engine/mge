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
     * A rectangle.
     */
    class MGEGRAPHICS_EXPORT rectangle
    {
    public:
        rectangle() {}

        rectangle(const point& top_left_, const extent& size_)
            : left(top_left_.x)
            , top(top_left_.y)
            , right(top_left_.x + size_.width)
            , bottom(top_left_.y + size_.height)
        {}

        rectangle(uint32_t left_,
                  uint32_t top_,
                  uint32_t right_,
                  uint32_t bottom_)
            : left(left_)
            , top(top_)
            , right(right_)
            , bottom(bottom_)
        {}

        ~rectangle() = default;

        rectangle(const rectangle&) = default;
        rectangle(rectangle&&) = default;
        rectangle& operator=(const rectangle&) = default;
        rectangle& operator=(rectangle&&) = default;

        inline void format(std::format_context& ctx) const
        {
            std::format_to(ctx.out(),
                           "rectangle{{left={}, top={}, right={}, bottom={}}}",
                           top,
                           left,
                           right,
                           bottom);
        }

        inline uint32_t width() const
        {
            return right - left;
        }

        inline uint32_t height() const
        {
            return bottom - top;
        }

        inline mge::extent extent() const
        {
            return mge::extent(width(), height());
        }

        inline mge::point top_left() const
        {
            return mge::point(left, top);
        }

        inline mge::point top_right() const
        {
            return mge::point(right, top);
        }

        inline mge::point bottom_left() const
        {
            return mge::point(left, bottom);
        }

        inline mge::point bottom_right() const
        {
            return mge::point(right, bottom);
        }

        inline uint64_t area() const
        {
            return static_cast<uint64_t>(width()) *
                   static_cast<uint64_t>(height());
        }

        uint32_t left{0};
        uint32_t top{0};
        uint32_t right{0};
        uint32_t bottom{0};
    };
} // namespace mge