// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/math/dllexport.hpp"
#include "mge/math/vec3.hpp"
#include <iosfwd>

/**
 * @file mge/math/aabox.hpp
 * @brief Axis-aligned box.
 */

namespace mge {

    /**
     * @brief An axis-aligned box.
     *
     * An axis aligned box is defined by its two corners.
     */
    class MGE_MATH_EXPORT aabox
    {
    public:
        /**
         * @brief Constructs an empty box.
         */
        aabox();
        /**
         * @brief Constructs an axis aligned box from two corners
         * @param corner1 first corner
         * @param corner2 second corner
         */
        aabox(const fvec3& corner1, const fvec3& corner2);

        /**
         * @brief Copy constructor.
         * @param b copied box
         */
        aabox(const aabox& b);

        /**
         * @brief Assignment operator.
         * @param b assigned box
         * @return @c *this
         */
        aabox& operator = (const aabox& b);

        /**
         * @brief Get box minimum corner.
         * @return minimum corner
         */
        const fvec3& min() const { return m_min; }
        /**
         * @brief Get box maximum corner.
         * @return maximum corner
         */
        const fvec3& max() const { return m_max; }

        /**
         * @brief Check whether vector is inside box.
         * @param v vector to check
         * @return @c true if inside box
         */
        bool contains(const fvec3& v) const;
    private:
        fvec3 m_min;
        fvec3 m_max;
    };

    /**
     * @brief Stream dump operator.
     * @param os output stream
     * @param b box
     * @return @c os
     */
    MGE_MATH_EXPORT std::ostream& operator <<(std::ostream& os, const aabox& b);
}
