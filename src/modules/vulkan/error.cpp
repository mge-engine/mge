// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "error.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"

namespace mge {
    MGE_USE_TRACE(VULKAN);
}

namespace mge::vulkan {
    MGE_DEFINE_EXCEPTION_CLASS(error);

    static const char* vkresult_message(VkResult rc)
    {
        switch (rc) {
        case VK_SUCCESS:
            return "Success";
        case VK_NOT_READY:
            return "A fence or query has not yet completed";
        case VK_TIMEOUT:
            return "A wait operation has not completed in the specified time";
        case VK_EVENT_SET:
            return "An event is signaled";
        case VK_EVENT_RESET:
            return "An event is unsignaled";
        case VK_INCOMPLETE:
            return "A return array was too small for the result";
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            return "A host memory allocation has failed";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            return "A device memory allocation has failed";
        case VK_ERROR_INITIALIZATION_FAILED:
            return "Initialization of an object could not be completed for "
                   "implementation-specific reasons";
        case VK_ERROR_DEVICE_LOST:
            return "The logical or physical device has been lost";
        case VK_ERROR_MEMORY_MAP_FAILED:
            return "Mapping of a memory object has failed";
        case VK_ERROR_LAYER_NOT_PRESENT:
            return "A requested layer is not present or could not be loaded";
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            return "A requested extension is not supported";
        case VK_ERROR_FEATURE_NOT_PRESENT:
            return "A requested feature is not supported";
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            return "The requested version of Vulkan is not supported by the "
                   "driver or is otherwise incompatible";
        case VK_ERROR_TOO_MANY_OBJECTS:
            return "Too many objects of the type have already been created";
        case VK_ERROR_FORMAT_NOT_SUPPORTED:
            return "A requested format is not supported on this device";
        case VK_ERROR_FRAGMENTED_POOL:
            return "A pool allocation has failed due to fragmentation of the "
                   "pool's memory";
        case VK_ERROR_UNKNOWN:
            return "An unknown error has occurred";
        case VK_ERROR_SURFACE_LOST_KHR:
            return "A surface is no longer available";
        case VK_SUBOPTIMAL_KHR:
            return "A swapchain no longer matches the surface properties "
                   "exactly, but can still be used";
        case VK_ERROR_OUT_OF_DATE_KHR:
            return "A surface has changed in such a way that it is no longer "
                   "compatible with the swapchain";
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
            return "The display used by a swapchain does not use the same "
                   "presentable image layout";
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
            return "The requested window is already connected to a "
                   "VkSurfaceKHR, or to some other non-Vulkan API";
        case VK_ERROR_VALIDATION_FAILED_EXT:
            return "A validation layer found an error";
        default:
            return "Unknown Vulkan error";
        }
    }

    error& error::set_info_from_vkresult(VkResult    rc,
                                         const char* file,
                                         int         line,
                                         const char* function)
    {
        set_info(mge::exception::source_file(file))
                .set_info(mge::exception::source_line(line))
                .set_info(mge::exception::called_function(function))
                .set_info(mge::exception::stack(mge::stacktrace()))
                .set_info(
                    mge::exception::type_name(mge::type_name<vulkan::error>()))
            << "Call to " << function << " failed: (" << (int)rc << ") "
            << vkresult_message(rc);
        return *this;
    }

    VkResult error::check_vkresult(VkResult    rc,
                                   const char* file,
                                   int         line,
                                   const char* function)
    {
        if (rc == VK_SUCCESS) {
            return rc;
        } else if (rc == VK_SUBOPTIMAL_KHR) {
            MGE_WARNING_TRACE(VULKAN)
                << file << ":" << line << ": "
                << "Call to " << function << " returned: (" << (int)rc << ") "
                << vkresult_message(rc);
            return rc;
        } else {
            MGE_ERROR_TRACE(VULKAN) << file << ":" << line << ": "
                                    << "Call to " << function << " failed: ("
                                    << (int)rc << ") " << vkresult_message(rc);
            vulkan::error err;
            throw err.set_info_from_vkresult(rc, file, line, function);
        }
    }

} // namespace mge::vulkan
