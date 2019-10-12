#pragma once
#include "mge/ui/dllexport.hpp"
#include "mge/ui/ui_fwd.hpp"
namespace mge {
    namespace ui {

        class MGE_UI_EXPORT menu_bar 
        {
        public:
            menu_bar();
            ~menu_bar();
        private:
            font_ref m_font;
        };
    }
}