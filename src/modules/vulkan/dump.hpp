#pragma once
#include "mge/core/details.hpp"
#include "mge/core/gist.hpp"
#include "vulkan.hpp"
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

    MGE_GIST_OUTPUT(VkLayerProperties)
    {
        os << "{Layer " << g.value->layerName << "}";
        return os;
    }

    MGE_GIST_OUTPUT(VkExtensionProperties)
    {
        os << "{Extension " << g.value->extensionName << "}";
        return os;
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

} // namespace mge