// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/rgba_color.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/graphics/color_names.hpp"
#include "mge/graphics/rgb_color.hpp"

namespace mge {

    rgba_color::rgba_color(unsigned int rgbavalue)
    {
        r = ((float)((rgbavalue >> 24) & 0xFF)) / 255.0f;
        g = ((float)((rgbavalue >> 16) & 0xFF)) / 255.0f;
        b = ((float)((rgbavalue >> 8) & 0xFF)) / 255.0f;
        a = ((float)(rgbavalue & 0xFF)) / 255.0f;
    }

    rgba_color::rgba_color(const rgb_color &c, float alpha)
    {
        r = c.r;
        g = c.g;
        b = c.b;
        a = alpha;
    }

    rgba_color::rgba_color(const char *name, float alpha) : a(alpha)
    {
        const color_name *c = find_color(name);
        if (c == 0) {
            MGE_THROW(mge::illegal_argument)
                << "Unknown color name \"" << name << "\"";
        }
        r = c->r;
        g = c->g;
        b = c->b;
    }

    bool rgba_color::operator==(const rgba_color &c) const
    {
        return r == c.r && g == c.g && b == c.b && a == c.a;
    }

    bool rgba_color::operator!=(const rgba_color &c) const
    {
        return r != c.r || g != c.g || b != c.b || a != c.a;
    }

    std::ostream &operator<<(std::ostream &o, const rgba_color &c)
    {
        return o << "rgba_color[r=" << c.r << ", g=" << c.g << ", b=" << c.b
                 << ", a=" << c.a << "]";
    }

} // namespace mge
