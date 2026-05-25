// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "render_context_base.hpp"

#ifdef MGE_OS_WINDOWS
#    include <Windows.h>
#endif

namespace mge::opengl {
    class render_system;

    class headless_render_context : public render_context_base
    {
    public:
        headless_render_context(mge::opengl::render_system& render_system_,
                                const mge::extent&          extent);
        ~headless_render_context() override;

        void           on_frame_present() override;
        mge::image_ref screenshot() override;

    private:
#ifdef MGE_OS_WINDOWS
        void create_hidden_window();
        void select_pixel_format();
        void create_glrc();

        HWND  m_hwnd{nullptr};
        HDC   m_hdc{nullptr};
        HGLRC m_hglrc{nullptr};
#endif
    };

} // namespace mge::opengl
