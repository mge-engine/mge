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
        {}

        window::~window() {}

        volatile bool s_window_class_created;

        void window::create_window_class()
        {
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
                //
            }
            s_window_class_created = true;
        }

        LRESULT CALLBACK window::wndproc(HWND hwnd, UINT umsg, WPARAM wparam,
                                         LPARAM lparam)
        {
            return 0;
        }

    } // namespace win32
} // namespace mge