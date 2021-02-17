// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include <cstdint>
#include <iosfwd>

namespace mge {
    /**
     * A point.
     */
    class MGEGRAPHICS_EXPORT point
    {
    public:
        point() : x(0), y(0) {}
        /**
         * @brief Create point.
         * @param x_ x coordinate
         * @param y_ y coordinate
         */
        point(uint32_t x_, uint32_t y_) : x(x_), y(y_) {}
        ~point() = default;

        point(const point &) = default;
        point(point &&)      = default;
        point &operator=(const point &) = default;
        point &operator=(point &&) = default;

        uint32_t x; //!< x coordinate
        uint32_t y; //!< y coordinate
    };

    /**
     * @brief Stream output operator.
     * @param os output stream
     * @param p  point
     * @return @c os
     */
    MGEGRAPHICS_EXPORT std::ostream &operator<<(std::ostream &os,
                                                const point & p);
} // namespace mge
