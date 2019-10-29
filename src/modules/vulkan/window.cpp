// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "window.hpp"
#include "render_context.hpp"

namespace vulkan {

    window::window(const mge::rectangle& rect,
                   const mge::window_options& options,
                   const system_config& config)
        :platform::window(rect, options)
    {
        create_render_context(config);
    }

    window::~window()
    {}

    void
    window::create_render_context(const system_config& config)
    {
        m_render_context = std::make_shared<::vulkan::render_context>(this, config);
    }
}
