// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/details.hpp"
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
                       static_cast<int>(p.format),
                       static_cast<int>(p.colorSpace));
    }

    MGE_DETAILS_OUTPUT(VkLayerProperties)
    {
        os << "{Layer "
           << "layerName=" << d.value->layerName << ", "
           << "description=" << d.value->description << ", "
           << "specVersion=";
        vulkan::dump_version(os, d.value->specVersion);
        os << ", "
           << "implementationVersion=";
        vulkan::dump_version(os, d.value->implementationVersion);
        os << "}";
        return os;
    }

    MGE_DETAILS_OUTPUT(VkExtensionProperties)
    {
        os << "{Extension extensionName=" << d.value->extensionName
           << ", specVersion=";
        vulkan::dump_version(os, d.value->specVersion);
        os << "}";
        return os;
    }

    MGE_DETAILS_OUTPUT(VkPhysicalDeviceProperties)
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
        os << "{PhysicalDevice deviceName=" << d.value->deviceName << "}";
        return os;
    }

    MGE_DETAILS_OUTPUT(VkSurfaceFormatKHR)
    {
        return os << "{VkSurfaceFormatKHR format=" << d.value->format
                  << ", colorSpace=" << d.value->colorSpace << " }";
    }

} // namespace mge