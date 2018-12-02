// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/window_options.hpp"
#include "mge/graphics/rectangle.hpp"
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
        virtual ~window();
        const rectangle& rect() const noexcept { return m_rect; }
        const window_options& options() const noexcept { return m_options; }
    protected:
        rectangle      m_rect;
        window_options m_options;
    };

}
