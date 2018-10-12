// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/graphics/dllexport.hpp"

#include <cstdint>
#include <algorithm>
#include <iosfwd>

#ifdef max
#  undef max
#endif

#ifdef min
#  undef min
#endif

namespace mge {

    /**
     * An extent, i.e. a width and a height.
     */
    class MGE_GRAPHICS_EXPORT extent
    {
    public:
        /**
         * Construct extent that has width and height of zero.
         */
        extent()
            :m_width(0)
            ,m_height(0)
        {}

        /**
         * Copy constructor.
         * @param e copied extent
         */
        extent(const extent& e)
            :m_width(e.m_width)
            ,m_height(e.m_height)
        {}

        /**
         * Construct from width and height.
         * @param width width
         * @param height height
         */
        extent(uint32_t width, uint32_t height)
            :m_width(width)
            ,m_height(height)
        {}

        /**
         * Get width.
         * @return width
         */
        uint32_t width() const { return m_width; }

        /**
         * Get height.
         * @return height
         */
        uint32_t height() const { return m_height; }

        /**
         * Get the area of the extent.
         * @return area of extent
         */
        uint32_t area() const { return m_width * m_height; }
    private:
        uint32_t m_width;
        uint32_t m_height;
    };

    MGE_GRAPHICS_EXPORT std::ostream& operator <<(std::ostream& os, const extent& e);
}

namespace std {

    inline mge::extent max(const mge::extent& a, const mge::extent& b)
    {
        return mge::extent(std::max(a.width(), b.width()),
                      std::max(a.height(), b.height()));
    }

    inline mge::extent min(const mge::extent& a, const mge::extent& b)
    {
        return mge::extent(std::min(a.width(), b.width()),
                      std::min(a.height(), b.height()));
    }
}
