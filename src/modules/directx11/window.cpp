// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "window.hpp"
#include "render_context.hpp"

namespace dx11 {
    window::window(const mge::rectangle &     rect,
                   const mge::window_options &options,
                   mge::render_system *system, const system_config &config)
        : mge::platform::window(rect, options), m_render_system(system)
    {
        create_render_context(config);
    }

    window::~window() {}

    void window::create_render_context(const system_config &config)
    {
        m_render_context = std::make_shared<dx11::render_context>(
            m_render_system, this, config);
    }

} // namespace dx11
