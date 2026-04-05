// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "mge/glfw/window.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"
#include "mge/input/key.hpp"
#include "mge/input/key_action.hpp"

namespace mge {
    MGE_DEFINE_TRACE(GLFW);
    MGE_USE_TRACE(GLFW);

    namespace glfw {

        window::window(mge::render_system&   render_system,
                       const mge::extent&    ext,
                       const window_options& options,
                       int                   client_api)
            : mge::window(render_system, ext, options)
            , m_handle(nullptr)
            , m_quit_listener(0)
            , m_input_listener(0)
        {
            MGE_DEBUG_TRACE(GLFW, "Creating GLFW window");

            if (!glfwInit()) {
                MGE_THROW(mge::runtime_exception) << "glfwInit failed";
            }

            glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
            glfwWindowHint(GLFW_CLIENT_API, client_api);
            if (client_api == GLFW_OPENGL_API) {
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
            }

            m_handle = glfwCreateWindow(static_cast<int>(ext.width),
                                        static_cast<int>(ext.height),
                                        "mge",
                                        nullptr,
                                        nullptr);
            if (!m_handle) {
                MGE_THROW(mge::runtime_exception)
                    << "Failed to create GLFW window";
            }

            glfwSetWindowUserPointer(m_handle, this);
            glfwSetWindowCloseCallback(m_handle, glfw_close_callback);
            glfwSetKeyCallback(m_handle, glfw_key_callback);

            m_quit_listener =
                mge::application::instance()->add_quit_listener([this] {
                    if (m_handle) {
                        glfwSetWindowShouldClose(m_handle, GLFW_TRUE);
                    }
                });

            m_input_listener = mge::application::instance()->add_input_listener(
                [this]() { this->process_input(); });
        }

        window::~window()
        {
            mge::application::instance()->remove_input_listener(
                m_input_listener);
            mge::application::instance()->remove_quit_listener(m_quit_listener);
            if (m_handle) {
                glfwDestroyWindow(m_handle);
                m_handle = nullptr;
            }
        }

        void window::on_show()
        {
            glfwShowWindow(m_handle);
        }

        void window::on_hide()
        {
            glfwHideWindow(m_handle);
        }

        void window::process_input()
        {
            glfwPollEvents();
        }

        void window::glfw_close_callback(GLFWwindow* w)
        {
            auto* self = static_cast<window*>(glfwGetWindowUserPointer(w));
            self->on_close();
        }

        static mge::key translate_key(int glfw_key)
        {
            if (glfw_key >= GLFW_KEY_0 && glfw_key <= GLFW_KEY_9) {
                return static_cast<mge::key>(glfw_key);
            }
            if (glfw_key >= GLFW_KEY_A && glfw_key <= GLFW_KEY_Z) {
                return static_cast<mge::key>(glfw_key);
            }
            switch (glfw_key) {
            case GLFW_KEY_SPACE:
                return mge::key::SPACE;
            case GLFW_KEY_ESCAPE:
                return mge::key::ESCAPE;
            case GLFW_KEY_ENTER:
                return mge::key::ENTER;
            case GLFW_KEY_TAB:
                return mge::key::TAB;
            case GLFW_KEY_BACKSPACE:
                return mge::key::BACKSPACE;
            case GLFW_KEY_INSERT:
                return mge::key::INSERT;
            case GLFW_KEY_DELETE:
                return mge::key::DELETE_KEY;
            case GLFW_KEY_HOME:
                return mge::key::HOME;
            case GLFW_KEY_END:
                return mge::key::END;
            case GLFW_KEY_PAGE_UP:
                return mge::key::PAGE_UP;
            case GLFW_KEY_PAGE_DOWN:
                return mge::key::PAGE_DOWN;
            case GLFW_KEY_UP:
                return mge::key::CURSOR_UP;
            case GLFW_KEY_DOWN:
                return mge::key::CURSOR_DOWN;
            case GLFW_KEY_LEFT:
                return mge::key::CURSOR_LEFT;
            case GLFW_KEY_RIGHT:
                return mge::key::CURSOR_RIGHT;
            case GLFW_KEY_CAPS_LOCK:
                return mge::key::CAPS_LOCK;
            case GLFW_KEY_SCROLL_LOCK:
                return mge::key::SCROLL_LOCK;
            case GLFW_KEY_NUM_LOCK:
                return mge::key::NUM_LOCK;
            case GLFW_KEY_PRINT_SCREEN:
                return mge::key::PRINT_SCREEN;
            case GLFW_KEY_PAUSE:
                return mge::key::PAUSE;
            case GLFW_KEY_F1:
                return mge::key::F1;
            case GLFW_KEY_F2:
                return mge::key::F2;
            case GLFW_KEY_F3:
                return mge::key::F3;
            case GLFW_KEY_F4:
                return mge::key::F4;
            case GLFW_KEY_F5:
                return mge::key::F5;
            case GLFW_KEY_F6:
                return mge::key::F6;
            case GLFW_KEY_F7:
                return mge::key::F7;
            case GLFW_KEY_F8:
                return mge::key::F8;
            case GLFW_KEY_F9:
                return mge::key::F9;
            case GLFW_KEY_F10:
                return mge::key::F10;
            case GLFW_KEY_F11:
                return mge::key::F11;
            case GLFW_KEY_F12:
                return mge::key::F12;
            case GLFW_KEY_LEFT_SHIFT:
                return mge::key::LEFT_SHIFT;
            case GLFW_KEY_LEFT_CONTROL:
                return mge::key::LEFT_CONTROL;
            case GLFW_KEY_LEFT_ALT:
                return mge::key::LEFT_ALT;
            case GLFW_KEY_RIGHT_SHIFT:
                return mge::key::RIGHT_SHIFT;
            case GLFW_KEY_RIGHT_CONTROL:
                return mge::key::RIGHT_CONTROL;
            case GLFW_KEY_RIGHT_ALT:
                return mge::key::RIGHT_ALT;
            case GLFW_KEY_MENU:
                return mge::key::MENU;
            case GLFW_KEY_KP_0:
                return mge::key::KP_0;
            case GLFW_KEY_KP_1:
                return mge::key::KP_1;
            case GLFW_KEY_KP_2:
                return mge::key::KP_2;
            case GLFW_KEY_KP_3:
                return mge::key::KP_3;
            case GLFW_KEY_KP_4:
                return mge::key::KP_4;
            case GLFW_KEY_KP_5:
                return mge::key::KP_5;
            case GLFW_KEY_KP_6:
                return mge::key::KP_6;
            case GLFW_KEY_KP_7:
                return mge::key::KP_7;
            case GLFW_KEY_KP_8:
                return mge::key::KP_8;
            case GLFW_KEY_KP_9:
                return mge::key::KP_9;
            case GLFW_KEY_KP_ADD:
                return mge::key::KP_ADD;
            case GLFW_KEY_KP_SUBTRACT:
                return mge::key::KP_SUBTRACT;
            case GLFW_KEY_KP_MULTIPLY:
                return mge::key::KP_MULTIPLY;
            case GLFW_KEY_KP_DIVIDE:
                return mge::key::KP_DIVIDE;
            case GLFW_KEY_KP_DECIMAL:
                return mge::key::KP_DECIMAL;
            case GLFW_KEY_KP_ENTER:
                return mge::key::KP_ENTER;
            default:
                return mge::key::INVALID;
            }
        }

        static mge::key_action translate_action(int glfw_action)
        {
            switch (glfw_action) {
            case GLFW_PRESS:
                return mge::key_action::PRESS;
            case GLFW_RELEASE:
                return mge::key_action::RELEASE;
            case GLFW_REPEAT:
                return mge::key_action::REPEAT;
            default:
                return mge::key_action::PRESS;
            }
        }

        void window::glfw_key_callback(
            GLFWwindow* w, int glfw_key, int scancode, int action, int mods)
        {
            auto* self = static_cast<window*>(glfwGetWindowUserPointer(w));
            auto  k = translate_key(glfw_key);
            if (k != mge::key::INVALID) {
                self->on_key_action(k, translate_action(action));
            }
        }

    } // namespace glfw
} // namespace mge
