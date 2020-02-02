// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/window_options.hpp"
#include "mge/graphics/rectangle.hpp"
#include "mge/input/input_handler.hpp"
#include "mge/application/application.hpp"
#include "mge/core/thread.hpp"
#include "mge/core/task_queue.hpp"

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
            , public async_executor
    {
    protected:
        window(const rectangle& dimension,
               const window_options& options);
    public:
        /// Context information passed on redraw.
        struct redraw_context
        {
            window          *window;            //!< Window redrawn.
            mge::thread     *display_thread;    //!< Window display thread.
            render_context&  render_context;    //!< Render context of window.
            float            interpolation;     //!< Interpolation to next frame.
        };

        /// Listener for close.
        using close_listener = std::function<void()>;
        /// Listener for closing.
        using closing_listener = std::function<bool()>;
        /// Listener functon for redraw.
        using redraw_listener = std::function<void(window::redraw_context&)>;

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
        extent extent() const noexcept { return m_rect.extent(); }

        /**
         * Access the render target.
         * @return window's render target
         */
        const render_context& render_context() const;

        /**
         * Access the render target.
         * @return window's render target
         */
        mge::render_context& render_context();

        /**
         * Set the redraw listener.
         * @param listener redraw listener
         */
        void set_redraw_listener(const redraw_listener& listener);

        /**
         * Clear the redraw listener.
         */
        void clear_redraw_listener();

        /**
         * Set the close listener.
         * @param listener close listener
         */
        void set_close_listener(const close_listener& listener);

        /**
         * Clear the close listener.
         */
        void clear_close_listener();

        /**
         * Set closing listener.
         * @param listener
         */
        void set_closing_listener(const closing_listener& listener);

        /**
         * Clear the closing listener.
         */
        void clear_closing_listener();

        /**
         * Shows the window.
         */
        void show();

        /**
         * Hides the window.
         */
        void hide();

        /**
         * Returns whether the window is visible (shown).
         * @return @c true if the window is visible
         */
        bool visible() const;

        /**
         * Closes the window.
         */
        void close();

        /**
         * Execute function, if possible in display thread.
         * @param f function to execute
         */
        void await(const void_function& f) override;

        class display_thread : public mge::thread
        {
        public:
            display_thread(window *w);
            virtual ~display_thread();
            void run() override;
            void set_quit();
        private:
            void exec_work_item();
            window *m_window;
            volatile bool m_quit;
        };
        friend class window::display_thread;
        virtual void assign_thread();

    protected:
        virtual void install_display_thread();
        void uninstall_display_thread();
        void refresh(mge::thread* display_thread, float interpolation);

        virtual void on_show() = 0;
        virtual void on_hide() = 0;
        virtual void request_close() = 0;
        void on_close();

        rectangle         m_rect;
        window_options    m_options;
        bool              m_visible;
        render_context_ref m_render_context;
        redraw_listener   m_redraw_listener;
        close_listener    m_close_listener;
        closing_listener  m_closing_listener;
        task_queue        m_tasks;
        std::shared_ptr<display_thread> m_display_thread;
        application::quit_listener_key_type m_quit_listener;
    };

}
