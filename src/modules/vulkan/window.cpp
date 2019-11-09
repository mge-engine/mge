// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "window.hpp"
#include "render_system.hpp"
#include "render_context.hpp"

namespace vulkan {

    window::window(render_system& system,
                   const mge::rectangle& rect,
                   const mge::window_options& options,
                   const system_config& config)
        :platform::window(rect, options)
        , m_render_system(system)
    {
        create_render_context(config);
    }

    window::~window()
    {}

    void window::create_render_context(const system_config& config)
    {
        m_render_context = std::make_shared<::vulkan::render_context>(this, 
                                                                      m_render_system,
                                                                      config);
    }
}
