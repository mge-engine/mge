/*
 * Copyright (c) 2017 by Alexander Schroeder
 */
#include "mge/graphics/rgb_color.hpp"
#include "mge/graphics/color_names.hpp"
#include "mge/core/stdexceptions.hpp"


namespace mge {


    rgb_color::rgb_color(unsigned int rgbvalue)
    {
        r = (float) ((rgbvalue >> 16) & 0xFF) / 255.0f;
        g = (float) ((rgbvalue >> 8) & 0xFF) / 255.0f;
        b = (float) (rgbvalue & 0xFF) / 255.0f;
    }

    rgb_color::rgb_color(const char *name)
    {
        const color_name *c = find_color(name);
        if (c == nullptr) {
            MGE_THROW(mge::illegal_argument)
                    << "Unknown color name \"" << name << "\"";
        } else {
            r = c->r;
            g = c->g;
            b = c->b;
        }
    }

    bool rgb_color::operator ==(const rgb_color& c) const
    {
        return r == c.r && g == c.g && b == c.b;
    }

    bool rgb_color::operator !=(const rgb_color& c) const
    {
        return r != c.r || g != c.g || b != c.b;
    }

    std::ostream&
    operator <<(std::ostream& o, const rgb_color& c)
    {
        return o << "rgb_color[r=" << c.r << ", g=" << c.g << ", b=" << c.b
                 << "]";
    }

    rgb_color operator "" _rgb(const char *name, size_t)
    {
        return rgb_color(name);
    }
}
