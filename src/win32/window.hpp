#pragma once
#include "mge/graphics/window.hpp"
namespace win32 {
    class window : public mge::window
    {
    protected:
        window(const mge::rectangle &rect,
               const mge::window_options &options);
    public:
        ~window();
        HWND hwnd() const { return m_hwnd; }

        static LRESULT CALLBACK wndproc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    private:
        static void create_window_class();

        HWND m_hwnd;
        static bool s_window_class_created;
    };
}
