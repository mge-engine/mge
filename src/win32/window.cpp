#include "win32/window.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/zero_memory.hpp"
#include "mge/core/log.hpp"
#include <windowsx.h>

#define MGE_CLASS_NAME ((LPCWSTR)L"mge")
#define WM_WANT_DESTROY (WM_USER + 1)

MGE_USE_LOG(WIN32);

namespace win32 {

    bool
    window::s_window_class_created;

    window::window(const mge::rectangle& rect,
                    const mge::window_options& options)
        : mge::window(rect, options)
        ,m_hwnd(0)
    {
        if(!s_window_class_created) {
            create_window_class();
        }
    }

    window::~window()
    {}

    LRESULT CALLBACK
    window::wndproc (HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
    {
        return DefWindowProcW(hwnd, umsg, wparam, lparam);
    }

    void
    window::create_window_class()
    {
        WNDCLASSEXW window_class;
        HINSTANCE module_handle;

        module_handle = GetModuleHandleW(NULL);
        mge::zero_memory(window_class);
        window_class.cbSize = sizeof(WNDCLASSEX);
        window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        window_class.lpfnWndProc = (WNDPROC)wndproc;
        window_class.cbWndExtra = sizeof(void *);
        window_class.hInstance = module_handle;
        window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
        window_class.lpszClassName = MGE_CLASS_NAME;
        window_class.hIcon = LoadIcon(module_handle, IDI_APPLICATION);
        window_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        window_class.hIconSm = LoadIcon(module_handle, IDI_APPLICATION);
        if (!RegisterClassExW(&window_class)) {
            MGE_THROW_CURRENT_SYSTEM_ERROR
                << mge::exception::message("Failed to register window class");
        }
        s_window_class_created = true;
    }

    void
    window::create_window()
    {
        MGE_DEBUG_LOG(WIN32) << "Create window";
        RECT window_rect = {
            rect().left(),
            rect().right(),
            rect().top(),
            rect().bottom()
        };
        DWORD style;
        DWORD exstyle;

        style = WS_OVERLAPPEDWINDOW
            | WS_CAPTION
            | WS_SYSMENU
            | WS_MINIMIZEBOX
            | WS_CLIPSIBLINGS
            | WS_CLIPCHILDREN;
        exstyle = WS_EX_APPWINDOW
            | WS_EX_WINDOWEDGE;

        AdjustWindowRectEx(&window_rect, style, 0, exstyle);
        m_hwnd = CreateWindowExW(exstyle,
                                 MGE_CLASS_NAME,
                                 L"",
                                 style,
                                 0,0,
                                 window_rect.right - window_rect.left,
                                 window_rect.bottom - window_rect.top,
                                 NULL,
                                 NULL,
                                 GetModuleHandle(NULL),
                                 this);
        MGE_DEBUG_LOG(WIN32) << "Window " << m_hwnd << " created";
        if(!m_hwnd) {
            MGE_THROW_CURRENT_SYSTEM_ERROR << mge::exception::message("Call to CreateWindowEx failed");
        }
    }

    window::msgthread::msgthread(window *w)
        :m_window(w)
    {}

    void
    window::msgthread::run()
    {
        HWND my_hwnd = nullptr;
        try {
            m_window->create_window();
            my_hwnd = m_window->hwnd();
            MSG msg;
            while(m_window->hwnd() && GetMessage(&msg, my_hwnd, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        } catch(const mge::exception& e) {
            MGE_ERROR_LOG(WIN32) << "Exception in window thread: " << e;
        }
        MGE_DEBUG_LOG(WIN32) << "Leave message thread for window " << my_hwnd;
    }

}
