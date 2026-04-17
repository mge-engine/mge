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
            glfwSetMouseButtonCallback(m_handle, glfw_mouse_button_callback);
            glfwSetCursorPosCallback(m_handle, glfw_cursor_pos_callback);
            glfwSetScrollCallback(m_handle, glfw_scroll_callback);
            glfwSetCharCallback(m_handle, glfw_char_callback);

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
            return static_cast<mge::key>(glfw_key);
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

        void window::glfw_mouse_button_callback(GLFWwindow* w,
                                                int         button,
                                                int         action,
                                                int         mods)
        {
            auto*  self = static_cast<window*>(glfwGetWindowUserPointer(w));
            double cx = 0;
            double cy = 0;
            glfwGetCursorPos(w, &cx, &cy);
            uint32_t x = static_cast<uint32_t>(cx);
            uint32_t y = static_cast<uint32_t>(cy);
            // GLFW buttons: 0=left, 1=right, 2=middle
            // mge buttons: 1=left, 2=right, 3=middle
            uint32_t mge_button = static_cast<uint32_t>(button) + 1;

            mge::mouse_action ma;
            switch (action) {
            case GLFW_PRESS:
                ma = mge::mouse_action::PRESS;
                break;
            case GLFW_RELEASE:
                ma = mge::mouse_action::RELEASE;
                break;
            default:
                return;
            }
            self->on_mouse_action(mge_button, ma, x, y);
        }

        void window::glfw_cursor_pos_callback(GLFWwindow* w, double x, double y)
        {
            auto* self = static_cast<window*>(glfwGetWindowUserPointer(w));
            self->on_mouse_move(static_cast<uint32_t>(x),
                                static_cast<uint32_t>(y));
        }

        void
        window::glfw_scroll_callback(GLFWwindow* w, double xoff, double yoff)
        {
            auto* self = static_cast<window*>(glfwGetWindowUserPointer(w));
            // Scale to match Win32 WHEEL_DELTA (120 per notch)
            self->on_mouse_wheel(static_cast<int32_t>(xoff * 120.0),
                                 static_cast<int32_t>(yoff * 120.0));
        }

        void window::glfw_char_callback(GLFWwindow* w, unsigned int codepoint)
        {
            auto* self = static_cast<window*>(glfwGetWindowUserPointer(w));
            self->on_character(static_cast<uint32_t>(codepoint));
        }

    } // namespace glfw
} // namespace mge
