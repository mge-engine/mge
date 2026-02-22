// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/format.hpp"
#include "mge/graphics/dllexport.hpp"

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
        inline rgba_color() {}

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

        /**
         * @brief Set the red component.
         *
         * @param r_ red value
         */
        void set_red(const float r_)
        {
            r = r_;
        }

        /**
         * @brief Set the green component.
         *
         * @param g_ green value
         */
        void set_green(const float g_)
        {
            g = g_;
        }

        /**
         * @brief Set the blue component.
         *
         * @param b_ blue value
         */
        void set_blue(const float b_)
        {
            b = b_;
        }

        /**
         * @brief Set the alpha component.
         *
         * @param a_ alpha value
         */
        void set_alpha(const float a_)
        {
            a = a_;
        }

        /**
         * @brief Red component value.
         *
         * @return red
         */
        float red() const
        {
            return r;
        }

        /**
         * @brief Green component value.
         *
         * @return green
         */
        float green() const
        {
            return g;
        }

        /**
         * @brief Blue component value.
         *
         * @return blue
         */
        float blue() const
        {
            return b;
        }

        /**
         * @brief Alpha component value.
         *
         * @return alpha
         */
        float alpha() const
        {
            return a;
        }

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
        const float* data() const noexcept
        {
            return &r;
        }

        float r{0.0f}; //!< red
        float g{0.0f}; //!< green
        float b{0.0f}; //!< blue
        float a{0.0f}; //!< alpha
    };

} // namespace mge

template <>
struct fmt::formatter<mge::rgba_color> : public fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::rgba_color& c, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(),
                       "rgba_color{{r={}, g={}, b={}, a={}}}",
                       c.r,
                       c.g,
                       c.b,
                       c.a);
        return ctx.out();
    }
};

namespace mge {
    inline std::ostream& operator<<(std::ostream& os, const mge::rgba_color& c)
    {
        fmt::print(os, "{}", c);
        return os;
    }
} // namespace mge