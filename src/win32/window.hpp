#pragma once
#include "mge/graphics/window.hpp"
#include "mge/core/thread.hpp"
#include <vector>
#include <functional>
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
        void wait_for_hwnd();
        void send_dtor_message();
        static void create_window_class();

        HWND m_hwnd;
        std::shared_ptr<msgthread> m_msgthread;
        std::vector<std::function<void ()>> m_messages;
        std::mutex m_messages_lock;
        static bool s_window_class_created;
    };
}
