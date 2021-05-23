// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/extent.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/point.hpp"
#include "mge/graphics/window_options.hpp"

#include <memory>
namespace mge {

    /**
     * @brief A window.
     */
    class MGEGRAPHICS_EXPORT window
        : public std::enable_shared_from_this<window>
    {
    protected:
        /**
         * @brief Create a window.
         * @param extent window size
         * @param options   window options
         */
        window(const extent &extent, const window_options &options);

    public:
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

    protected:
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
        bool                    m_visible;
    };
} // namespace mge
