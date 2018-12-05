#include "win32/window.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/zero_memory.hpp"
#include "mge/core/log.hpp"
#include "mge/core/crash.hpp"
#include "mge/application/application.hpp"
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

        m_msgthread = std::make_shared<msgthread>(this);
        // starting the "message thread" which takes care on
        // hwnd life cycle and message processing
        m_msgthread->start();
        // but we have to wait for the hwnd actually to
        // exist, as subclasses want to execute other
        // functions, like gl/dx context creation and
        // expect a valid hwnd when this function returns
        wait_for_hwnd();

        // if the application is quitting, send a message
        // to destroy the window
        HWND hwnd = this->m_hwnd;
        mge::application::instance().add_quit_listener([=]{
            PostMessage(hwnd, WM_WANT_DESTROY, 0, 0);
        });
    }

    window::~window()
    {
        // someone destroys the window, and wants to tear down
        // the message processing
        // unfortunately, windows likes to handle all window
        // ops in one thread, so we need to send a friendly
        // reminder to the message processor and let it
        // do the hard work
        if(m_msgthread) {
            if(mge::this_thread::get_id() == m_msgthread->get_id()) {
                // well, this would be essentially calling this windows
                // dtor from the message handling itself
                // as logical message handling is deferred to the
                // polling thread,
                MGE_ERROR_LOG(WIN32) << "Message thread cannot destroy own window!";
                mge::crash();
            } else {
                // only if we can still join the thread
                // and did not suffer a window create catastrophe
                if(m_msgthread->joinable() && m_hwnd) {
                    send_dtor_message();
                    m_msgthread->join();
                }
            }
        }
    }

    void window::send_dtor_message()
    {
        BOOL rc = PostMessage(m_hwnd, WM_WANT_DESTROY, 0, 0);
        if (!rc) {
            MGE_ERROR_LOG(WIN32) << "Call to PostMessage failed: " << rc;
        }
    }


    void
    window::wait_for_hwnd()
    {
        while(!m_hwnd) {
            MGE_DEBUG_LOG(WIN32) << "Wait for window creation";
            mge::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

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
