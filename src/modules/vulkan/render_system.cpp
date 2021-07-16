// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/render_system.hpp"
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

    class render_system : public mge::render_system
    {
    public:
        render_system()
        {
            MGE_INFO_TRACE(VULKAN) << "Creating Vulkan render system";
        }
        ~render_system() = default;

        mge::window_ref create_window(const mge::extent &        extent,
                                      const mge::window_options &options)
        {
            // auto ref = std::make_shared<mge::vulkan::window>(extent,
            // options);
            mge::window_ref ref;
            return ref;
        }

        monitor_collection monitors()
        {
#ifdef MGE_OS_WINDOWS
            return mge::win32::monitor::all_monitors();
#endif
        }
    };

    MGE_REGISTER_IMPLEMENTATION(render_system, mge::render_system, vulkan, vk);
} // namespace mge::vulkan