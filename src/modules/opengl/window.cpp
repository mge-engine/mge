// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "window.hpp"
#include "render_context.hpp"

namespace opengl {

    window::window(const mge::rectangle& rect,
                   const mge::window_options& options,
                   bool debug)
        : mge::platform::window(rect, options)
    {
        create_render_context(debug);
    }

    window::~window()
    {}

    void
    window::create_render_context(bool debug)
    {
        auto t = std::make_shared<opengl::render_context>(this, debug);
        m_render_context = t;
    }
}
