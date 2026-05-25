// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/core/mutex.hpp"
#include "mge/core/thread.hpp"
#include "render_context_base.hpp"
#include "window.hpp"
#ifndef MGE_OS_WINDOWS
#    define GLFW_INCLUDE_NONE
#    include <GLFW/glfw3.h>
#endif

namespace mge::opengl {

    class render_context : public render_context_base
    {
    public:
        render_context(mge::opengl::render_system& render_system_,
                       mge::opengl::window*        context_window);
        ~render_context() override;

        void           on_frame_present() override;
        mge::image_ref screenshot() override;

#ifdef MGE_OS_WINDOWS
        HDC dc() const
        {
            return m_hdc;
        }
#endif

    private:
#ifdef MGE_OS_WINDOWS
        void select_pixel_format();
        void create_primary_glrc();

        mge::opengl::window*        m_window;
        HWND                        m_hwnd;
        HDC                         m_hdc;
        HGLRC                       m_primary_hglrc;
        mge::mutex                  m_lock;
        std::map<thread::id, HGLRC> m_thread_glrcs;
#else
        mge::opengl::window* m_window;
        GLFWwindow*          m_glfw_window;
#endif
    };

} // namespace mge::opengl
