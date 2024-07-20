// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "render_context.hpp"
#include "enumerate.hpp"
#include "error.hpp"
#include "render_system.hpp"
#include "window.hpp"

#include "mge/core/trace.hpp"
namespace mge {
    MGE_USE_TRACE(VULKAN);
}

namespace mge::vulkan {

    render_context::render_context(render_system& render_system_,
                                   window&        window_)
        : m_render_system(render_system_)
        , m_window(window_)
    {
        try {
            create_surface();
            create_device();
            resolve_device_functions();
            get_device_queue();
            fetch_surface_capabilities();
        } catch (...) {
            teardown();
            throw;
        }
    }

    render_context::~render_context() { teardown(); }

    mge::index_buffer_ref render_context::create_index_buffer(data_type dt,
                                                              size_t data_size,
                                                              void*  data)
    {
        mge::index_buffer_ref result;
        return result;
    }

    mge::vertex_buffer_ref render_context::create_vertex_buffer(
        const vertex_layout& layout, size_t data_size, void* data)
    {
        mge::vertex_buffer_ref result;
        return result;
    }

    mge::shader_ref render_context::create_shader(shader_type t)
    {
        mge::shader_ref result;
        return result;
    }

    mge::program_ref render_context::create_program()
    {
        mge::program_ref result;
        return result;
    }

    mge::command_list_ref render_context::create_command_list()
    {
        mge::command_list_ref result;
        return result;
    }

    mge::texture_ref render_context::create_texture(texture_type type)
    {
        mge::texture_ref result;
        return result;
    }

    void render_context::create_surface()
    {
#ifdef MGE_OS_WINDOWS
        MGE_DEBUG_TRACE(VULKAN) << "Create Vulkan surface (Win32)";
        if (!m_render_system.vkCreateWin32SurfaceKHR) {
            MGE_THROW(error)
                << "Cannot create surface: vkCreateWin32SurfaceKHR function "
                   "missing";
        }
        VkWin32SurfaceCreateInfoKHR create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        // no need to release module handle
        create_info.hinstance = GetModuleHandle(nullptr);
        create_info.hwnd = m_window.hwnd();
        create_info.flags = 0; // must be 0, reserved
        CHECK_VK_CALL(
            m_render_system.vkCreateWin32SurfaceKHR(m_render_system.instance(),
                                                    &create_info,
                                                    nullptr,
                                                    &m_surface));

#else
#    error Missing port
#endif
    }

    void render_context::create_device()
    {
        MGE_DEBUG_TRACE(VULKAN) << "Create logical device";
        VkDeviceQueueCreateInfo queue_create_info{};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex =
            m_render_system.graphics_queue_index();
        queue_create_info.queueCount = 1;
        float queue_priority = 1.0f;
        queue_create_info.pQueuePriorities = &queue_priority;

        VkPhysicalDeviceFeatures device_features{};

        VkDeviceCreateInfo device_create_info{};
        device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        device_create_info.pQueueCreateInfos = &queue_create_info;
        device_create_info.queueCreateInfoCount = 1;
        device_create_info.pEnabledFeatures = &device_features;

        std::vector<const char*> device_extensions;
        device_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

        std::vector<const char*> device_layers;
        if (m_render_system.debug()) {
            device_layers.push_back("VK_LAYER_KHRONOS_validation");
        }
        device_create_info.ppEnabledLayerNames = device_layers.data();
        device_create_info.enabledLayerCount =
            static_cast<uint32_t>(device_layers.size());
        device_create_info.enabledExtensionCount =
            static_cast<uint32_t>(device_extensions.size());
        device_create_info.ppEnabledExtensionNames = device_extensions.data();

        CHECK_VK_CALL(
            m_render_system.vkCreateDevice(m_render_system.physical_device(),
                                           &device_create_info,
                                           nullptr,
                                           &m_device));
        MGE_DEBUG_TRACE(VULKAN) << "Created logical device: " << m_device;
    }

    void render_context::teardown()
    {
        m_queue = VK_NULL_HANDLE;

        if (m_device && vkDestroyDevice) {
            vkDestroyDevice(m_device, nullptr);
            m_device = VK_NULL_HANDLE;
        }

        clear_functions();

        if (m_surface != VK_NULL_HANDLE &&
            m_render_system.vkDestroySurfaceKHR) {
            m_render_system.vkDestroySurfaceKHR(m_render_system.instance(),
                                                m_surface,
                                                nullptr);
            m_surface = VK_NULL_HANDLE;
        }
    }

    static void* resolve_device_function(void*                   original,
                                         PFN_vkGetDeviceProcAddr getDeviceProc,
                                         VkDevice                device,
                                         const char*             name)
    {
        void* result = original;
        auto  ptr = getDeviceProc(device, name);
        if (ptr) {
            MGE_DEBUG_TRACE(VULKAN)
                << "Replace " << name << ": " << (void*)(original) << " by "
                << (void*)ptr;
            result = ptr;
        }
        return result;
    }

    void render_context::resolve_device_functions()
    {
        MGE_DEBUG_TRACE(VULKAN) << "Resolve device functions";
#ifdef MGE_COMPILER_MSVC
#    pragma warning(push)
#    pragma warning(disable : 4191)
#endif
#define RESOLVE(X)                                                             \
    this->X = reinterpret_cast<decltype(this->X)>(                             \
        resolve_device_function((void*)m_render_system.X,                      \
                                m_render_system.vkGetDeviceProcAddr,           \
                                m_device,                                      \
                                #X));

#define BASIC_INSTANCE_FUNCTION(X)
#define INSTANCE_FUNCTION(X)
#define DEVICE_FUNCTION(X) RESOLVE(X)

#include "vulkan_core.inc"
#ifdef MGE_OS_WINDOWS
#    include "vulkan_win32.inc"
#endif

#undef BASIC_INSTANCE_FUNCTION
#undef INSTANCE_FUNCTION
#undef DEVICE_FUNCTION
#undef RESOLVE
#ifdef MGE_COMPILER_MSVC
#    pragma warning(pop)
#endif
    }

    void render_context::clear_functions()
    {
        MGE_DEBUG_TRACE(VULKAN) << "Clear device functions";
#ifdef MGE_COMPILER_MSVC
#    pragma warning(push)
#    pragma warning(disable : 4191)
#endif
#define RESOLVE(X) this->X = nullptr;

#define BASIC_INSTANCE_FUNCTION(X) RESOLVE(X)
#define INSTANCE_FUNCTION(X) RESOLVE(X)
#define DEVICE_FUNCTION(X) RESOLVE(X)

#include "vulkan_core.inc"
#ifdef MGE_OS_WINDOWS
#    include "vulkan_win32.inc"
#endif

#undef BASIC_INSTANCE_FUNCTION
#undef INSTANCE_FUNCTION
#undef DEVICE_FUNCTION
#undef RESOLVE
#ifdef MGE_COMPILER_MSVC
#    pragma warning(pop)
#endif
    }

    void render_context::get_device_queue()
    {
        MGE_DEBUG_TRACE(VULKAN) << "Get device queue";
        vkGetDeviceQueue(m_device,
                         m_render_system.graphics_queue_index(),
                         0,
                         &m_queue);
    }

    void render_context::fetch_surface_capabilities()
    {
        MGE_DEBUG_TRACE(VULKAN) << "Fetch surface capabilities";
        CHECK_VK_CALL(m_render_system.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            m_render_system.physical_device(),
            m_surface,
            &m_surface_capabilities));
        enumerate(
            [this](uint32_t* count, VkSurfaceFormatKHR* data) {
                CHECK_VK_CALL(
                    m_render_system.vkGetPhysicalDeviceSurfaceFormatsKHR(
                        m_render_system.physical_device(),
                        m_surface,
                        count,
                        data));
            },
            m_surface_formats);
        MGE_DEBUG_TRACE(VULKAN)
            << "Found " << m_surface_formats.size() << " surface formats";
        for (const auto& format : m_surface_formats) {
            MGE_DEBUG_TRACE(VULKAN)
                << "    " << format.format << "/" << format.colorSpace;
        }
        enumerate(
            [this](uint32_t* count, VkPresentModeKHR* data) {
                CHECK_VK_CALL(
                    m_render_system.vkGetPhysicalDeviceSurfacePresentModesKHR(
                        m_render_system.physical_device(),
                        m_surface,
                        count,
                        data));
            },
            m_surface_present_modes);

        MGE_DEBUG_TRACE(VULKAN)
            << "Found " << m_surface_present_modes.size() << " present modes";
        for (const auto& f : m_surface_present_modes) {
            MGE_DEBUG_TRACE(VULKAN) << "    " << f;
        }
    }

} // namespace mge::vulkan