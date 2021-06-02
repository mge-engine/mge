// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/win32/window.hpp"
#include "mge/core/trace.hpp"
#include "mge/core/zero_memory.hpp"

#define MGE_CLASS_NAME  ((LPCWSTR)L"mge")
#define WM_WANT_DESTROY (WM_USER + 1)

namespace mge {

#define _OLD_WIN32 WIN32
#undef WIN32
    MGE_USE_TRACE(WIN32);
#define WIN32 _OLD_WIN32

    namespace win32 {

        window::window(const mge::extent &extent, const window_options &options)
            : mge::window(extent, options), m_hwnd(0)
        {
            create_window_class();
            create_window();
        }

        window::~window() {}

        volatile bool s_window_class_created;

        void window::create_window_class()
        {
            if (s_window_class_created) {
                return;
            }

            MGE_DEBUG_TRACE(WIN32) << "Create window class";

            WNDCLASSEXW window_class;
            HINSTANCE   module_handle;

            module_handle              = GetModuleHandle(NULL);
            window_class.cbSize        = sizeof(WNDCLASSEX);
            window_class.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
            window_class.lpfnWndProc   = (WNDPROC)wndproc;
            window_class.cbWndExtra    = sizeof(void *);
            window_class.hInstance     = module_handle;
            window_class.hCursor       = LoadCursor(NULL, IDC_ARROW);
            window_class.lpszClassName = MGE_CLASS_NAME;
            window_class.hIcon = LoadIcon(module_handle, IDI_APPLICATION);
            window_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
            window_class.hIconSm = LoadIcon(module_handle, IDI_APPLICATION);
            if (!RegisterClassExW(&window_class)) {
                MGE_ERROR_TRACE(WIN32) << "Registering window class failed";
            }
            s_window_class_created = true;
        }

        void window::create_window()
        {
            MGE_DEBUG_TRACE(WIN32) << "Create window";

            RECT window_rect = {0, 0, (LONG)extent().width,
                                (LONG)extent().height};

            DWORD style, exstyle;

            style = WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU |
                    WS_MINIMIZEBOX | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
            exstyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

            AdjustWindowRectEx(&window_rect, style, 0, exstyle);

            m_hwnd = CreateWindowExW(exstyle, MGE_CLASS_NAME, L"", style, 0, 0,
                                     window_rect.right - window_rect.left,
                                     window_rect.bottom - window_rect.top, NULL,
                                     NULL, GetModuleHandle(NULL), this);

            MGE_DEBUG_TRACE(WIN32) << "Window " << m_hwnd << " created";

            if (application::instance()) {
                m_process_input_listener =
                    application::instance()->add_input_listener(
                        [&]() { this->process_input(); });
            } else {
                MGE_THROW(mge::illegal_state)
                    << "Cannot create window without application instance";
            }
        }

        void window::process_input()
        {
            MSG msg;
            if (m_hwnd && GetMessage(&msg, m_hwnd, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        void window::on_show()
        {
            if (m_hwnd) {
                MGE_DEBUG_TRACE(WIN32) << "ShowWindow(SW_SHOW) " << m_hwnd;
                ShowWindow(m_hwnd, SW_SHOW);
                BringWindowToTop(m_hwnd);
                SetForegroundWindow(m_hwnd);
                SetFocus(m_hwnd);
            }
        }

        void window::on_hide()
        {
            if (m_hwnd) {
                MGE_DEBUG_TRACE(WIN32) << "ShowWindow(SW_HIDE) " << m_hwnd;
                ShowWindow(m_hwnd, SW_HIDE);
            }
        }

        LRESULT CALLBACK window::wndproc(HWND hwnd, UINT umsg, WPARAM wparam,
                                         LPARAM lparam)
        {
            window *w = (window *)GetWindowLongPtr(hwnd, 0);
            if (w == nullptr) {
                switch (umsg) {
                case WM_NCCREATE: {
                    CREATESTRUCT *cs = (CREATESTRUCT *)lparam;
                    SetWindowLongPtr(hwnd, 0, (LONG_PTR)cs->lpCreateParams);
                    break;
                }
                default:
                    break;
                }
                return DefWindowProcW(hwnd, umsg, wparam, lparam);
            } else {
                switch (umsg) {
                case WM_CLOSE:
                    w->on_close();
                    break;
                case WM_WANT_DESTROY:
                    MGE_DEBUG_TRACE(WIN32)
                        << "Destroy of window " << hwnd << "requested";
                    w->m_hwnd = 0;
                    DestroyWindow(hwnd);
                    break;
                default:
                    return DefWindowProcW(hwnd, umsg, wparam, lparam);
                }
            }
            return 0;
        }
    } // namespace win32
} // namespace mge