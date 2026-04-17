// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/application/application.hpp"
#include "mge/graphics/window.hpp"
#include "mge/input/mouse_action.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace mge {
    namespace glfw {

        class window : public mge::window
        {
        public:
            window(mge::render_system&   render_system,
                   const mge::extent&    extent,
                   const window_options& options,
                   int                   client_api = GLFW_NO_API);
            ~window();

            GLFWwindow* handle() const noexcept
            {
                return m_handle;
            }

        protected:
            void on_show() override;
            void on_hide() override;

        private:
            static void glfw_close_callback(GLFWwindow* w);
            static void glfw_key_callback(
                GLFWwindow* w, int key, int scancode, int action, int mods);
            static void glfw_mouse_button_callback(GLFWwindow* w,
                                                   int         button,
                                                   int         action,
                                                   int         mods);
            static void
            glfw_cursor_pos_callback(GLFWwindow* w, double x, double y);
            static void
            glfw_scroll_callback(GLFWwindow* w, double xoff, double yoff);
            static void glfw_char_callback(GLFWwindow*  w,
                                           unsigned int codepoint);
            void        process_input();

            GLFWwindow*                     m_handle;
            application::quit_listener_key  m_quit_listener;
            application::input_listener_key m_input_listener;
        };

    } // namespace glfw
} // namespace mge
