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
    window::window(render_system&             render_system_,
                   const mge::extent&         extent,
                   const mge::window_options& options)
        : platform::window(extent, options)
        , m_render_system(render_system_)
    {
        MGE_DEBUG_TRACE(VULKAN) << "Create window";
        create_render_context();
    }

    window::~window() {}

    void window::create_render_context()
    {
        MGE_DEBUG_TRACE(VULKAN) << "Create render context";
        auto context =
            std::make_shared<::mge::vulkan::render_context>(m_render_system,
                                                            *this);
        context->init_swap_chain();
        m_render_context = context;
    }

} // namespace mge::vulkan