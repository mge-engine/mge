// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "render_system.hpp"
#include "mge/core/parameter.hpp"
#include "mge/core/trace.hpp"

#ifdef MGE_OS_WINDOWS
#    include "mge/win32/monitor.hpp"
#endif

namespace mge {
    MGE_USE_TRACE(VULKAN);
    MGE_DEFINE_PARAMETER_WITH_DEFAULT(
        bool, vulkan, debug, "Enable Vulkan debug mode", false);

} // namespace mge

namespace mge::vulkan {

    render_system::render_system() {}

    render_system::~render_system() {}

    mge::window_ref
    render_system::create_window(const mge::extent&         extent,
                                 const mge::window_options& options)
    {
        return nullptr;
    }

    std::span<mge::monitor_ref> render_system::monitors()
    {
        return std::span<mge::monitor_ref>();
    }
} // namespace mge::vulkan