// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/ui/dllexport.hpp"
#include "mge/ui/ui_fwd.hpp"
namespace mge {
    namespace ui {

        class font_implementation;
        MGE_DECLARE_REF(font_implementation);

        class MGE_UI_EXPORT font
        {
        public:
            enum class style {
                NORMAL,
                ITALIC
            };

            enum class weight {
                LIGHT,
                NORMAL,
                BOLD
            };

            font(const char *family,
                 font::style style = font::style::NORMAL,
                 font::weight weight = font::weight::NORMAL,
                 uint32_t size = 12);
            font(const font&) = default;
            font(font&&) = default;
            ~font() = default;
        private:
            font_implementation_ref m_implementation;
        };
    }
}
