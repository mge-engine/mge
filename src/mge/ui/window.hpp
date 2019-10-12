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
                VISIBLE     = 1<<0,
                COLLAPSED   = 1<<1
            };

            window();
            ~window();

            point position() const { return m_rect.position(); }
            mge::extent extent() const { return m_rect.extent(); }
            const std::string& title() const { return m_title; }
            menu_bar_ref menu_bar() const { return m_menu_bar; }


        private:
            std::string    m_title;
            mge::rectangle m_rect;
            menu_bar_ref   m_menu_bar;
            uint8_t        m_status;
        };
    }
}