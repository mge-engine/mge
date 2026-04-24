// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "error.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"
#include <fmt/format.h>

namespace mge {
    MGE_USE_TRACE(VULKAN);
}

namespace mge::vulkan {
    MGE_DEFINE_EXCEPTION_CLASS(error);

    static std::string vkresult_message(VkResult rc)
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
        case VK_ERROR_OUT_OF_POOL_MEMORY:
            return "A pool memory allocation has failed";
        case VK_ERROR_INVALID_EXTERNAL_HANDLE:
            return "An external handle is not a valid handle of the specified "
                   "type";
        case VK_ERROR_FRAGMENTATION:
            return "A descriptor pool creation has failed due to "
                   "fragmentation";
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
            return "A buffer creation or memory allocation failed because the "
                   "requested address is not available";
        case VK_PIPELINE_COMPILE_REQUIRED:
            return "A requested pipeline creation would have required "
                   "compilation, but the application requested compilation to "
                   "not be performed";
        case VK_ERROR_INVALID_SHADER_NV:
            return "One or more shaders failed to compile or link";
        case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
            return "The requested image usage is not supported";
        case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:
            return "The requested video picture layout is not supported";
        case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:
            return "The requested video profile operation is not supported";
        case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:
            return "The requested video profile format is not supported";
        case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:
            return "The requested video profile codec is not supported";
        case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
            return "The requested video standard version is not supported";
        case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
            return "The requested DRM format modifier plane layout is invalid";
        case VK_ERROR_NOT_PERMITTED_KHR:
            return "A requested operation is not permitted";
        case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
            return "An operation on a swapchain created with exclusive "
                   "full-screen access failed as it did not have exclusive "
                   "full-screen access";
        case VK_THREAD_IDLE_KHR:
            return "A deferred operation is not complete but there is no "
                   "work remaining to assign to additional threads";
        case VK_THREAD_DONE_KHR:
            return "A deferred operation is not complete but there is no "
                   "work remaining to assign to this thread";
        case VK_OPERATION_DEFERRED_KHR:
            return "A deferred operation was requested and at least some of "
                   "the work was deferred";
        case VK_OPERATION_NOT_DEFERRED_KHR:
            return "A deferred operation was requested and no work was "
                   "deferred";
        case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR:
            return "The provided video std parameters are invalid";
        case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
            return "An image creation failed because internal resources "
                   "required for compression are exhausted";
        case VK_INCOMPATIBLE_SHADER_BINARY_EXT:
            return "The provided binary shader code is not compatible with "
                   "this device";
        case VK_PIPELINE_BINARY_MISSING_KHR:
            return "A pipeline binary was required but was not found";
        case VK_ERROR_NOT_ENOUGH_SPACE_KHR:
            return "There is not enough space in the buffer to write the "
                   "result";
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
            return fmt::format("Unrecognized VkResult code: {}", (int)rc);
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
            MGE_WARNING_TRACE(VULKAN,
                              "Call to {} returned: ({}) {}",
                              function,
                              (int)rc,
                              vkresult_message(rc));
            return rc;
        } else {
            MGE_ERROR_TRACE(VULKAN,
                            "Call to {} failed: ({}) {}",
                            function,
                            (int)rc,
                            vkresult_message(rc));
            vulkan::error err;
            throw err.set_info_from_vkresult(rc, file, line, function);
        }
    }

} // namespace mge::vulkan
