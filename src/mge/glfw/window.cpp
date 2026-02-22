// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "mge/glfw/window.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"

namespace mge {
    MGE_USE_TRACE(GLFW);

    namespace glfw {

        window::window(mge::render_system&   render_system,
                       const mge::extent&    ext,
                       const window_options& options)
            : mge::window(render_system, ext, options)
            , m_handle(nullptr)
            , m_quit_listener(0)
        {
            MGE_DEBUG_TRACE(GLFW, "Creating GLFW window");

            if (!glfwInit()) {
                MGE_THROW(mge::runtime_exception) << "glfwInit failed";
            }

            glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

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

            m_quit_listener =
                mge::application::instance()->add_quit_listener([this] {
                    if (m_handle) {
                        glfwSetWindowShouldClose(m_handle, GLFW_TRUE);
                    }
                });
        }

        window::~window()
        {
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

        void window::glfw_close_callback(GLFWwindow* w)
        {
            auto* self = static_cast<window*>(glfwGetWindowUserPointer(w));
            self->on_close();
        }

    } // namespace glfw
} // namespace mge
