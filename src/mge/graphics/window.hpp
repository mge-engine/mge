// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/window_options.hpp"
#include "mge/graphics/rectangle.hpp"
#include "mge/input/input_handler.hpp"
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
            , public input_handler
    {
    protected:
        window(const rectangle& dimension,
               const window_options& options);
    public:
        typedef std::function<void()> close_listener;
        typedef std::function<bool()> closing_listener;
        typedef std::function<void(render_target&, float)> redraw_listener;

        virtual ~window();

        /**
         * Access window rectangle.
         * @return window rectangle
         */
        const rectangle& rect() const noexcept { return m_rect; }

        /**
         * Access window options.
         * @return window options
         */
        const window_options& options() const noexcept { return m_options; }
        extent extent() const;
        const render_target& render_target() const;
        mge::render_target& render_target();
        void set_redraw_listener(const redraw_listener& listener);

        /**
         * Shows the window.
         */
        void show();

        void hide();
        bool visible() const;
        void close();

        /**
         * Refreshes the window content. Calls an attached redraw
         * listener.
         * @param interpolation interpolation time to new frame
         */
        void refresh(float interpolation);
    protected:
        virtual void on_show() = 0;
        virtual void on_hide() = 0;

        rectangle         m_rect;
        window_options    m_options;
        bool              m_visible;
        render_target_ref m_render_target;
        redraw_listener   m_redraw_listener;
    };

}
