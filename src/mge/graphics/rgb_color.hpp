// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include <iosfwd>

namespace mge {

    /**
     * An RGB color.
     */
    class MGEGRAPHICS_EXPORT rgb_color
    {
    public:
        /**
         * Constructor. Intializes to zero.
         */
        inline rgb_color() : r(0.0f), g(0.0f), b(0.0f) {}

        /**
         * Constructor.
         * @param rgbvalue rgb value (like 0xFFAAEE)
         */
        explicit rgb_color(unsigned int rgbvalue);

        /**
         * Copy constructor.
         * @param c copied color value
         */
        inline rgb_color(const rgb_color &c) = default;

        /**
         * Create RGB color from color name.
         * @param name color name
         */
        explicit rgb_color(const char *name);

        /**
         * Construct (greyscale) color from single value.
         * @param value color value
         */
        explicit inline rgb_color(const float value)
            : r(value), g(value), b(value)
        {}

        /**
         * Creates a color
         * @param r_ red
         * @param g_ green
         * @param b_ blue
         */
        inline rgb_color(float r_, float g_, float b_) : r(r_), g(g_), b(b_) {}

        /**
         * Initialize from array.
         * @param values color value array
         */
        inline rgb_color(const float *values)
            : r(values[0]), g(values[1]), b(values[2])
        {}

        /**
         * Assignment operator.
         * @param c color to assign.
         * @return @c *this
         */
        rgb_color &operator=(const rgb_color &c) = default;

        /**
         * Set red component,
         * @param r_ red value
         */
        void set_red(const float r_)
        {
            r = r_;
            return;
        }

        /**
         * Set green component.
         * @param g_ green component
         */
        void set_green(const float g_) { g = g_; }

        /**
         * Set blue component.
         * @param b_ blue component
         */
        void set_blue(const float b_) { b = b_; }

        /**
         * Get red component.
         * @return red component
         */
        float get_red() const { return r; }

        /**
         * Get green component.
         * @return green component
         */
        float get_green() const { return g; }

        /**
         * Get blue component.
         * @return blue component
         */
        float get_blue() const { return b; }

        /**
         * Comparison operator.
         * @param c compared color
         * @return @c true if this color equals @c c
         */
        bool operator==(const rgb_color &c) const;

        /**
         * Comparison operator.
         * @param c compared color
         * @return @c true if this color is different from @c c
         */
        bool operator!=(const rgb_color &c) const;

        float r; //!< red
        float g; //!< green
        float b; //!< blue
    };

    MGEGRAPHICS_EXPORT rgb_color operator"" _rgb(const char *, size_t);

    /**
     * Stream dump operator.
     * @param o output stream
     * @param c color to dump
     * @return @c o
     */
    MGEGRAPHICS_EXPORT std::ostream &operator<<(std::ostream &   o,
                                                const rgb_color &c);
} // namespace mge
