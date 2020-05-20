// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "window.hpp"
#include "render_context.hpp"

namespace opengl {

    window::window(const mge::rectangle &     rect,
                   const mge::window_options &options,
                   mge::render_system *system, bool debug)
        : mge::platform::window(rect, options), m_render_system(system)
    {
        create_render_context(debug);
    }

    window::~window() {}

    void window::create_render_context(bool debug)
    {
        auto t = std::make_shared<opengl::render_context>(m_render_system, this,
                                                          debug);
        m_render_context = t;
    }
} // namespace opengl
