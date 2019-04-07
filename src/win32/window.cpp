// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
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

    static mge::key translate_key(WPARAM wparam, LPARAM lparam)
    {
        if(wparam == VK_PROCESSKEY) {
            return mge::key::INVALID;
        }
        if(wparam >= (WPARAM)mge::key::ZERO && wparam <= (WPARAM)mge::key::NINE) {
            return (mge::key)wparam;
        }
        if(wparam >= (WPARAM)mge::key::A && wparam <= (WPARAM)mge::key::Z) {
            return (mge::key)wparam;
        }
        switch(wparam) {
        case VK_SPACE:
            return mge::key::SPACE;
        case VK_F1:
            return mge::key::F1;
        case VK_F2:
            return mge::key::F2;
        case VK_F3:
            return mge::key::F3;
        case VK_F4:
            return mge::key::F4;
        case VK_F5:
            return mge::key::F5;
        case VK_F6:
            return mge::key::F6;
        case VK_F7:
            return mge::key::F7;
        case VK_F8:
            return mge::key::F8;
        case VK_F9:
            return mge::key::F9;
        case VK_F10:
            return mge::key::F10;
        case VK_F11:
            return mge::key::F11;
        case VK_F12:
            return mge::key::F12;
        case VK_INSERT:
            return mge::key::INSERT;
        case VK_DELETE:
            return mge::key::DELETE_KEY;
        case VK_HOME:
            return mge::key::HOME;
        case VK_END:
            return mge::key::END;
        case VK_PRIOR:
            return mge::key::PAGE_UP;
        case VK_NEXT:
            return mge::key::PAGE_DOWN;
        case VK_UP:
            return mge::key::CURSOR_UP;
        case VK_DOWN:
            return mge::key::CURSOR_DOWN;
        case VK_LEFT:
            return mge::key::CURSOR_LEFT;
        case VK_RIGHT:
            return mge::key::CURSOR_RIGHT;
        case VK_CAPITAL:
            return mge::key::CAPS_LOCK;
        case VK_SCROLL:
            return  mge::key::SCROLL_LOCK;
        case VK_PAUSE:
            return mge::key::PAUSE;
        case VK_PRINT:
            return mge::key::PRINT_SCREEN;
        case VK_NUMLOCK:
            return mge::key::NUM_LOCK;
        case VK_LSHIFT:
            return mge::key::LEFT_SHIFT;
        case VK_RSHIFT:
            return mge::key::RIGHT_SHIFT;
        case VK_LCONTROL:
            return mge::key::LEFT_CONTROL;
        case VK_RCONTROL:
            return mge::key::RIGHT_CONTROL;
        case VK_LMENU:
            return mge::key::LEFT_ALT;
        case VK_RMENU:
            return mge::key::RIGHT_ALT;
        case VK_MENU:
            return mge::key::MENU;
        case VK_NUMPAD0:
            return mge::key::KP_0;
        case VK_NUMPAD1:
            return mge::key::KP_1;
        case VK_NUMPAD2:
            return mge::key::KP_2;
        case VK_NUMPAD3:
            return mge::key::KP_3;
        case VK_NUMPAD4:
            return mge::key::KP_4;
        case VK_NUMPAD5:
            return mge::key::KP_5;
        case VK_NUMPAD6:
            return mge::key::KP_6;
        case VK_NUMPAD7:
            return mge::key::KP_7;
        case VK_NUMPAD8:
            return mge::key::KP_8;
        case VK_NUMPAD9:
            return mge::key::KP_9;
        case VK_ESCAPE:
            return mge::key::ESCAPE;
        case 13:
            return mge::key::ENTER;
        }

        MGE_DEBUG_LOG(WIN32) << "Bad key: " << wparam;

        return mge::key::INVALID;
    }


    window::window(const mge::rectangle& rect,
                    const mge::window_options& options)
        : mge::window(rect, options)
        ,m_hwnd(0)
        ,m_update_listener(0)
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
                    MGE_DEBUG_LOG(WIN32) << "Joining message thread (r";
                    send_dtor_message();
                    m_msgthread->join();
                } else if(m_msgthread->joinable()) {
                    // window already gone, but thread still alive
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
        window *w = (window *) GetWindowLongPtr(hwnd, 0);
        if (w == nullptr) {
            switch(umsg) {
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
            switch(umsg) {
            case WM_CLOSE: {
                w->add_message([w]{
                    w->on_close();
                });
                break;
            }
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN: {
                mge::key k = translate_key(wparam, lparam);
                if(k == mge::key::INVALID) {
                    break;
                }
                w->add_message([w,k] {
                    w->on_key_action(k, mge::key_action::PRESS);
                });
                break;
            }
            case WM_KEYUP:
            case WM_SYSKEYUP: {
                mge::key k = translate_key(wparam, lparam);
                if(k == mge::key::INVALID) {
                    break;
                }
                w->add_message([w,k]{
                    w->on_key_action(k, mge::key_action::RELEASE);
                });
                break;
            }
            case WM_LBUTTONDOWN:
                w->add_message([w, lparam]{
                    w->on_mouse_action(1,
                                       mge::mouse_action::PRESS,
                                       GET_X_LPARAM(lparam),
                                       GET_Y_LPARAM(lparam));
                });
                break;
            case WM_LBUTTONUP:
                w->add_message([w, lparam]{
                    w->on_mouse_action(1,
                                       mge::mouse_action::RELEASE,
                                       GET_X_LPARAM(lparam),
                                       GET_Y_LPARAM(lparam));
                });
                break;
            case WM_LBUTTONDBLCLK:
                w->add_message([w, lparam]{
                    w->on_mouse_action(1,
                                       mge::mouse_action::MOUSE_ACTION_DOUBLE_CLICK,
                                       GET_X_LPARAM(lparam),
                                       GET_Y_LPARAM(lparam));
                });
                break;
            case WM_RBUTTONDOWN:
                w->add_message([w, lparam]{
                    w->on_mouse_action(2,
                                       mge::mouse_action::PRESS,
                                       GET_X_LPARAM(lparam),
                                       GET_Y_LPARAM(lparam));
                });
                break;
            case WM_RBUTTONUP:
                w->add_message([w, lparam]{
                    w->on_mouse_action(2,
                                       mge::mouse_action::RELEASE,
                                       GET_X_LPARAM(lparam),
                                       GET_Y_LPARAM(lparam));
                });
                break;
            case WM_RBUTTONDBLCLK:
                w->add_message([w, lparam]{
                    w->on_mouse_action(2,
                                       mge::mouse_action::MOUSE_ACTION_DOUBLE_CLICK,
                                       GET_X_LPARAM(lparam),
                                       GET_Y_LPARAM(lparam));
                });
                break;
            case WM_MOUSEMOVE:
                w->add_message([w, lparam]{
                    w->on_mouse_move(GET_X_LPARAM(lparam),
                                     GET_Y_LPARAM(lparam));
                });
                break;
            case WM_CHAR:
                if(wparam == 0x0D || wparam == 0x09 || wparam == 0x08) {
                    w->add_message([w, wparam]{
                        switch(wparam) {
                        case 0x0D:
                            w->on_key_action(mge::key::ENTER, mge::key_action::PRESS);
                            break;
                        case 0x08:
                            w->on_key_action(mge::key::BACKSPACE, mge::key_action::PRESS);
                            break;
                        case 0x09:
                            w->on_key_action(mge::key::TAB, mge::key_action::PRESS);
                            break;
                        }
                        w->on_character((uint32_t)wparam);
                        switch(wparam) {
                        case 0x0D:
                            w->on_key_action(mge::key::ENTER, mge::key_action::RELEASE);
                            break;
                        case 0x08:
                            w->on_key_action(mge::key::BACKSPACE, mge::key_action::RELEASE);
                            break;
                        case 0x09:
                            w->on_key_action(mge::key::TAB, mge::key_action::RELEASE);
                            break;
                        }
                    });
                } else {
                    w->add_message([w, wparam]{
                        w->on_character((uint32_t)wparam);
                    });
                }
                break;
            case WM_WANT_DESTROY:
                MGE_DEBUG_LOG(WIN32) << "Destruction of window " << hwnd << " requested";
                w->m_hwnd = nullptr;
                DestroyWindow(hwnd);
                break;
            default:
                return DefWindowProcW(hwnd, umsg, wparam, lparam);
            }
        }
        return 0;
    }

    void
    window::add_message(std::function<void()>&& f)
    {
        std::lock_guard<decltype(m_messages_lock)> guard(m_messages_lock);
        m_messages.push_back(f);
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
            MGE_THROW_CURRENT_SYSTEM_ERROR << "Failed to register window class";
        }
        s_window_class_created = true;
    }

    void
    window::create_window()
    {
        MGE_DEBUG_LOG(WIN32) << "Create window";
        RECT window_rect = {
            rect().left(),
            rect().top(),
            rect().right(),
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
            MGE_THROW_CURRENT_SYSTEM_ERROR << "Call to CreateWindowEx failed";
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

    void
    window::on_show()
    {
        ShowWindow(m_hwnd, SW_SHOW);
        BringWindowToTop(m_hwnd);
        SetForegroundWindow(m_hwnd);
        SetFocus(m_hwnd);
        m_update_listener = mge::application::instance().add_update_listener([&](uint64_t){
            this->poll_events();
        });
    }

    void
    window::on_hide()
    {
        mge::application::instance().remove_update_listener(m_update_listener);
        m_update_listener = 0;
        ShowWindow(m_hwnd, SW_HIDE);
    }

    void
    window::poll_events()
    {
        std::lock_guard<decltype(m_messages_lock)> guard(m_messages_lock);
        for(const auto& f : m_messages) {
            f();
        }
        m_messages.clear();
    }

    void
    window::request_close()
    {
        if(m_hwnd) {
            PostMessage(m_hwnd, WM_CLOSE, 0, 0);
        }
    }

}
