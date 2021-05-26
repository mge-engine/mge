// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/void_function.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/extent.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/point.hpp"
#include "mge/graphics/window_options.hpp"
#include "mge/input/input_handler.hpp"

#include <memory>
namespace mge {

    /**
     * @brief A window.
     */
    class MGEGRAPHICS_EXPORT window
        : public std::enable_shared_from_this<window>,
          public input_handler
    {
    protected:
        /**
         * @brief Create a window.
         * @param extent window size
         * @param options   window options
         */
        window(const extent &extent, const window_options &options);

    public:
        /** @brief Listener type for close action.
         */
        using close_listener = void_function;

        /**
         * @brief Listener to check on closing action.
         *
         * This listener will be called on close action. If the listener
         * returns @c false, the window will not be closed.
         */
        using closing_listener = std::function<bool()>;

        /**
         * Get window position.
         * @return window position.
         */
        virtual point position() const;

        /**
         * Get window extent.
         * @return window extent
         */
        virtual mge::extent extent() const;

        /**
         * Shows the window.
         */
        void show();

        /**
         * Hides the window.
         */
        void hide();

        /**
         * Get whether window is visible.
         * @return @c true if visible
         */
        bool visible() const;

        virtual ~window();

        /**
         * @brief Access render context of window.
         *
         * @return render context
         */
        mge::render_context &render_context();

        /**
         * @brief Access render context of window.
         *
         * @return render context
         */
        const mge::render_context &render_context() const;

        /**
         * @brief Set the close listener.
         *
         * @param listener callback to call when window is closed
         */
        void set_close_listener(const close_listener &listener);

        /**
         * @brief Clears the currently set close listener.
         */
        void clear_close_listener();

        /**
         * @brief Set the closing listener.
         *
         * The closing listener can abort the window close by returning @c
         * false.
         *
         * @param listener callback to call when window is requested to be
         * closed
         */
        void set_closing_listener(const closing_listener &listener);

        /**
         * @brief Clears the currently set closing listener.
         */
        void clear_closing_listener();

    protected:
        /**
         * @brief Called when window is closed.
         *
         * @return @c true if window is closed
         */
        bool on_close();

        /**
         * Called when window is shown.
         */
        virtual void on_show() = 0;

        /**
         * Called when window is hidden.
         */
        virtual void on_hide() = 0;

        mge::point              m_position;
        mge::extent             m_extent;
        mge::render_context_ref m_render_context;
        close_listener          m_close_listener;
        closing_listener        m_closing_listener;
        bool                    m_visible;
    };
} // namespace mge
