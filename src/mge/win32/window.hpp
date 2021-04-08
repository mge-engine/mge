// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.

#include "mge/graphics/window.hpp"
#include "mge/application/application.hpp"

#include <windows.h>

namespace mge {
    namespace win32 {

        class window : public mge::window
        {
        public:
            window(const mge::extent &extent, const window_options &options);
            ~window();

            HWND hwnd() const noexcept { return m_hwnd; }

        private:
            static LRESULT CALLBACK wndproc(HWND hwnd, UINT umsg, WPARAM wparam,
                                            LPARAM lparam);

            void        create_window();
            static void create_window_class();
            void        process_input();

            HWND m_hwnd;
            application::input_listener_key m_process_input_listener;
        };

    } // namespace win32
} // namespace mge