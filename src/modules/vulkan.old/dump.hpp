// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/details.hpp"
#include "mge/core/enum.hpp"
#include "mge/core/gist.hpp"
#include "vulkan.hpp"
#include "vulkan_dump.h"
#include <iostream>

namespace mge {

    namespace vulkan {
        inline void dump_version(std::ostream& os, uint32_t v)
        {
            auto major = VK_API_VERSION_MAJOR(v);
            auto minor = VK_API_VERSION_MINOR(v);
            auto patch = VK_API_VERSION_PATCH(v);
            os << major << "." << minor << "." << patch;
            return;
        }
    } // namespace vulkan

    inline void gist(const VkLayerProperties& p, std::format_context& ctx)
    {
        std::format_to(ctx.out(), "{{Layer {}}}", p.layerName);
    }

    inline void gist(const VkExtensionProperties& p, std::format_context& ctx)
    {
        std::format_to(ctx.out(), "{{Extension {}}}", p.extensionName);
    }

    inline void gist(const VkPhysicalDeviceProperties& p,
                     std::format_context&              ctx)
    {
        std::format_to(ctx.out(), "{{PhysicalDevice {}}}", p.deviceName);
    }

    inline void gist(const VkSurfaceFormatKHR& p, std::format_context& ctx)
    {
        std::format_to(ctx.out(),
                       "{{VkSurfaceFormatKHR format={}, colorSpace={}}}",
                       static_cast<uint32_t>(p.format),
                       static_cast<uint32_t>(p.colorSpace));
    }

    inline void details(const VkLayerProperties& p, std::format_context& ctx)
    {
        auto spec_major = VK_API_VERSION_MAJOR(p.specVersion);
        auto spec_minor = VK_API_VERSION_MINOR(p.specVersion);
        auto spec_patch = VK_API_VERSION_PATCH(p.specVersion);

        auto impl_major = VK_API_VERSION_MAJOR(p.implementationVersion);
        auto impl_minor = VK_API_VERSION_MINOR(p.implementationVersion);
        auto impl_patch = VK_API_VERSION_PATCH(p.implementationVersion);

        std::format_to(ctx.out(),
                       "{{Layer layerName={}, description={}, "
                       "specVersion={}.{}.{}, implementationVersion={}.{}.{}}}",
                       p.layerName,
                       p.description,
                       spec_major,
                       spec_minor,
                       spec_patch,
                       impl_major,
                       impl_minor,
                       impl_patch);
    }

    inline void details(const VkExtensionProperties& p,
                        std::format_context&         ctx)
    {
        auto spec_major = VK_API_VERSION_MAJOR(p.specVersion);
        auto spec_minor = VK_API_VERSION_MINOR(p.specVersion);
        auto spec_patch = VK_API_VERSION_PATCH(p.specVersion);

        std::format_to(ctx.out(),
                       "{{Extension extensionName={}, specVersion={}.{}.{} }}",
                       p.extensionName,
                       spec_major,
                       spec_minor,
                       spec_patch);
    }

    inline void details(const VkPhysicalDeviceProperties& p,
                        std::format_context&              ctx)
    {
        // TODO: print more as needed
        // uint32_t                            apiVersion;
        // uint32_t                            driverVersion;
        // uint32_t                            vendorID;
        // uint32_t                            deviceID;
        // VkPhysicalDeviceType                deviceType;
        // char deviceName[VK_MAX_PHYSICAL_DEVICE_NAME_SIZE]; uint8_t
        // pipelineCacheUUID[VK_UUID_SIZE]; VkPhysicalDeviceLimits limits;
        // VkPhysicalDeviceSparseProperties    sparseProperties;

        auto api_major = VK_VERSION_MAJOR(p.apiVersion);
        auto api_minor = VK_VERSION_MINOR(p.apiVersion);
        auto api_patch = VK_VERSION_PATCH(p.apiVersion);

        std::format_to(ctx.out(),
                       "{{PhysicalDevice deviceName={}, apiVersion={}.{}.{} }}",
                       p.deviceName,
                       api_major,
                       api_minor,
                       api_patch);
    }

    inline void details(const VkSurfaceFormatKHR& p, std::format_context& ctx)
    {
        std::format_to(ctx.out(),
                       "{{VkSurfaceFormatKHR format={}, colorSpace={}}}",
                       static_cast<int>(p.format),
                       static_cast<int>(p.colorSpace));
    }

} // namespace mge