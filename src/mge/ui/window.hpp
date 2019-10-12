#pragma once
#include "mge/ui/dllexport.hpp"
#include "mge/ui/ui_fwd.hpp"
#include "mge/graphics/rectangle.hpp"
namespace mge {
    namespace ui {
        /**
         * A window.
         */
        class window 
        {
        public:
            enum status : uint8_t {
                VISIBLE     = 1 << 0,
                COLLAPSED   = 1 << 1
            };

            enum properties : uint16_t {
                TITLE           = 1 << 0,
                MENU_BAR        = 1 << 1,
                RESIZABLE       = 1 << 2,
                MOVABLE         = 1 << 3,
                CLOSEABLE       = 1 << 4,
                COLLAPSIBLE     = 1 << 5,
                MINMAX          = 1 << 6
            };

            window();
            ~window();

            point position() const { return m_rect.position(); }
            mge::extent extent() const { return m_rect.extent(); }
            const std::string& title() const { return m_title; }
            menu_bar_ref menu_bar() const { return m_menu_bar; }

            void collapse();
            void expand();
        private:
            std::string    m_title;
            mge::rectangle m_rect;
            menu_bar_ref   m_menu_bar;
            uint16_t       m_properties;
            uint8_t        m_status;
        };
    }
}