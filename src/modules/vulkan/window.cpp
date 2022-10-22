// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "window.hpp"
#include "render_system.hpp"

namespace mge::vulkan {
    window::window(render_system&             render_system_,
                   const mge::extent&         extent,
                   const mge::window_options& options)
        : platform::window(extent, options)
        , m_render_system(render_system_)
    {
        create_render_context();
    }

    window::~window() {}

    void window::create_render_context() {}
} // namespace mge::vulkan