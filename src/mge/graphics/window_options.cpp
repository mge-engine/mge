// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/window_options.hpp"

namespace mge {

    window_options &window_options::set_option(option o, bool value)
    {
        if (o != window_options::NONE) {
            uint32_t oval = (uint32_t)o;
            oval--;
            m_options[oval] = value;
        }
        return *this;
    }

    bool window_options::option_set(window_options::option o) const
    {
        if (o == window_options::NONE) {
            return false;
        } else {
            uint32_t oval = (uint32_t)o;
            oval--;
            return m_options[oval];
        }
    }

    window_options window_options::standard_options()
    {
        return window_options()
            .set_option(RESIZABLE)
            .set_option(BORDER)
            .set_option(TITLE);
    }

    window_options window_options::fullscreen_options()
    {
        return window_options().set_option(FULLSCREEN);
    }

} // namespace mge
