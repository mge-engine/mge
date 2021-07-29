// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "render_system.hpp"
#include "mge/core/trace.hpp"
// #include "window.hpp"
#include <memory>

#ifdef MGE_OS_WINDOWS
#    include "mge/win32/monitor.hpp"
#endif

namespace mge {
    MGE_USE_TRACE(VULKAN);
}

namespace mge::vulkan {

    render_system::render_system()
    {
        MGE_INFO_TRACE(VULKAN) << "Creating Vulkan render system";
    }

    mge::window_ref
    render_system::create_window(const mge::extent &        extent,
                                 const mge::window_options &options)
    {
        // auto ref = std::make_shared<mge::vulkan::window>(extent,
        // options);
        mge::window_ref ref;
        return ref;
    }

    mge::render_system::monitor_collection render_system::monitors()
    {
#ifdef MGE_OS_WINDOWS
        return mge::win32::monitor::all_monitors();
#endif
    }

    MGE_REGISTER_IMPLEMENTATION(render_system, mge::render_system, vulkan, vk);
} // namespace mge::vulkan