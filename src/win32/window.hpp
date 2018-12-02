#pragma once
#include "mge/graphics/window.hpp"
#include "mge/core/thread.hpp"
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

        class msgthread : public mge::thread
        {
        public:
            msgthread(window *w);
            ~msgthread() = default;
            void run() override;
        private:
            window *m_window;
        };

    private:
        void create_window();
        static void create_window_class();

        HWND m_hwnd;
        static bool s_window_class_created;
    };
}
