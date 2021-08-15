// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include <iosfwd>

namespace mge {

    class rgb_color;

    /**
     * An RGBA color.
     */
    class MGEGRAPHICS_EXPORT rgba_color
    {
    public:
        /**
         * Default constructor.
         */
        inline rgba_color()
            : r(0.0f)
            , g(0.0f)
            , b(0.0f)
            , a(0.0f)
        {}

        /**
         * Constructor
         * @param rgbavalue RGBA value (like 0xFFAAEE77 (alpha is 0x77 here))
         */
        explicit rgba_color(unsigned int rgbavalue);

        /**
         * Copy constructor.
         * @param c copied color value
         */
        inline rgba_color(const rgba_color& c) = default;

        rgba_color(const rgb_color& c, float alpha = 1.0f);

        /**
         * Create RGBA color from color name.
         * @param name color name
         * @param alpha alpha value
         */
        rgba_color(const char* name, float alpha = 1.0f);

        /**
         * Construct (greyscale) color from single value.
         * @param value color value
         * @param alpha alpha value
         */
        inline rgba_color(const float value, const float alpha)
            : r(value)
            , g(value)
            , b(value)
            , a(alpha)
        {}

        /**
         * Creates a color
         * @param r_ red
         * @param g_ green
         * @param b_ blue
         * @param a_ alpha
         */
        inline rgba_color(float r_, float g_, float b_, float a_)
            : r(r_)
            , g(g_)
            , b(b_)
            , a(a_)
        {}

        /**
         * Initialize from array.
         * @param values color value array
         */
        inline rgba_color(const float* values)
            : r(values[0])
            , g(values[1])
            , b(values[2])
            , a(values[3])
        {}

        /**
         * Assignment operator.
         * @param c color to assign.
         * @return @c *this
         */
        inline rgba_color& operator=(const rgba_color& c) = default;

        void set_red(const float r_) { r = r_; }

        void set_green(const float g_) { g = g_; }

        void set_blue(const float b_) { b = b_; }

        void set_alpha(const float a_) { a = a_; }

        float red() const { return r; }

        float green() const { return g; }

        float blue() const { return b; }

        float alpha() const { return a; }

        /**
         * Compare with other color.
         * @param c color for comparison
         * @return @c true if colors equal
         */
        bool operator==(const rgba_color& c) const;

        /**
         * Compare with other color.
         * @param c color for comparison
         * @return @c true if colors not equal
         */
        bool operator!=(const rgba_color& c) const;

        /**
         * Access color as array of floats.
         * @return color values
         */
        const float* data() const noexcept { return &r; }

        float r; //!< red
        float g; //!< green
        float b; //!< blue
        float a; //!< alpha
    };

    /**
     * Stream dump operator.
     * @param o output stream
     * @param c color to dump
     * @return @c o
     */
    MGEGRAPHICS_EXPORT std::ostream& operator<<(std::ostream&     o,
                                                const rgba_color& c);

} // namespace mge
