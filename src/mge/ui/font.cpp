// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#include "mge/ui/font.hpp"
// HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts
namespace mge {
    namespace ui {

        class font_implementation
        {
        };

        font::font(const char *family,
                   font::style style,
                   font::weight weight,
                   uint32_t size)
        {}

    }
}
