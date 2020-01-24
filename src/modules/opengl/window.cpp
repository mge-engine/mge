// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "window.hpp"
#include "render_context.hpp"

namespace opengl {

    window::window(const mge::rectangle& rect,
                   const mge::window_options& options)
        : mge::platform::window(rect, options)
    {
        create_render_context();
    }

    window::~window()
    {}

    void
    window::create_render_context()
    {
        auto t = std::make_shared<opengl::render_context>(this);
        m_render_context = t;
    }
}
