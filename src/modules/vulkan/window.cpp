// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "window.hpp"
#include "render_context.hpp"
#include "render_system.hpp"
namespace mge {
    MGE_USE_TRACE(VULKAN);
}

namespace mge::vulkan {
    window::window(render_system&             system,
                   const mge::extent&         extent,
                   const mge::window_options& options)
        : platform::window(system, extent, options)
    {
        MGE_DEBUG_TRACE(VULKAN, "Create window");
        create_render_context(system);
    }

    window::~window() {}

    void window::create_render_context(render_system& system)
    {
        MGE_DEBUG_TRACE(VULKAN, "Create render context");
        auto context =
            std::make_shared<::mge::vulkan::render_context>(system,
                                                            *this);
        m_render_context = context;
    }

} // namespace mge::vulkan