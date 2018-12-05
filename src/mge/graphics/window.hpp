// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/window_options.hpp"
#include "mge/graphics/rectangle.hpp"

#include <functional>

namespace mge {


    /**
     * A window. A window interfaces with the operating
     * system to display information presented on its
     * device context, and to receive input when it has the
     * focus.
     *
     * A window may be _windowed_ (have a title and handles to
     * minimize/maximize/close etc.) or may be a full screen
     * window that has no borders at all.
     */
    class MGE_GRAPHICS_EXPORT window
            : public std::enable_shared_from_this<window>
    {
    protected:
        window(const rectangle& dimension,
               const window_options& options);
    public:
        typedef std::function<void()> close_listener;
        typedef std::function<bool()> closing_listener;
        typedef std::function<void(render_target&, float)> redraw_listener;

        virtual ~window();
        const rectangle& rect() const noexcept { return m_rect; }
        const window_options& options() const noexcept { return m_options; }
        extent extent() const;
        const render_target& render_target() const;
        mge::render_target& render_target();

        void show();
        void hide();
        bool visible() const;
        void close();
        void refresh(float interpolation);
    protected:
        virtual void on_show() = 0;
        virtual void on_hide() = 0;

        rectangle         m_rect;
        window_options    m_options;
        bool              m_visible;
        render_target_ref m_render_target;
    };

}
