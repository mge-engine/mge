#pragma once
#include "mge/ui/dllexport.hpp"
#include <cstdint>
namespace mge {
    namespace ui {
        class MGE_UI_EXPORT menu 
        {
        public:
            enum status : uint8_t {
                COLLAPSED =  1 << 0
            };

            menu();
            ~menu();

            void collapse();
            void expand();
        private:
            uint8_t m_status;
        };
    }
}