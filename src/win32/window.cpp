#include "win32/window.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/zero_memory.hpp"
#include <windowsx.h>

#define MOGE_CLASS_NAME ((LPCWSTR)L"mge")
#define WM_WANT_DESTROY (WM_USER + 1)

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
        window_class.lpszClassName = MOGE_CLASS_NAME;
        window_class.hIcon = LoadIcon(module_handle, IDI_APPLICATION);
        window_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        window_class.hIconSm = LoadIcon(module_handle, IDI_APPLICATION);
        if (!RegisterClassExW(&window_class)) {
            MGE_THROW_CURRENT_SYSTEM_ERROR
                << mge::exception::message("Failed to register window class");
        }
        s_window_class_created = true;
    }

}
