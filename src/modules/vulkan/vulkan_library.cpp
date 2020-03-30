// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "vulkan_library.hpp"
#include "mge/core/log.hpp"
#include "mge/core/stdexceptions.hpp"
MGE_USE_LOG(VULKAN);

namespace vulkan {
    vulkan_library::vulkan_library()
    {
#ifdef MGE_OS_WINDOWS
        const char *vulkan_library_name = "vulkan-1.dll";
#else
#    error Unsupported Platform
#endif
        MGE_DEBUG_LOG(VULKAN)
            << "Loading vulkan library " << vulkan_library_name;
        m_library = mge::shared_library::load(vulkan_library_name);

        vkGetInstanceProcAddr =
            reinterpret_cast<decltype(vkGetInstanceProcAddr)>(
                m_library->symbol("vkGetInstanceProcAddr"));
        if (!vkGetInstanceProcAddr) {
            MGE_THROW(mge::illegal_state)
                << "Entry point \"vkGetInstanceProcAddr\" not found in "
                << vulkan_library_name;
        }
    }

    vulkan_library::~vulkan_library() {}
} // namespace vulkan