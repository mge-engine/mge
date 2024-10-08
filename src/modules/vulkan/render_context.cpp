// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "render_context.hpp"
#include "command_list.hpp"
#include "enumerate.hpp"
#include "error.hpp"
#include "frame_command_list.hpp"
#include "index_buffer.hpp"
#include "program.hpp"
#include "render_system.hpp"
#include "shader.hpp"
#include "swap_chain.hpp"
#include "vertex_buffer.hpp"
#include "window.hpp"

#include "mge/core/trace.hpp"
namespace mge {
    MGE_USE_TRACE(VULKAN);
}

namespace mge::vulkan {

    render_context::render_context(render_system& render_system_,
                                   window&        window_)
        : m_render_system(render_system_.shared_from_this())
        , m_window(window_)
    {
        try {
            create_surface();
            create_device();
            resolve_device_functions();
            create_allocator();
            get_device_queue();
            fetch_surface_capabilities();
            choose_extent();
            create_swap_chain();
            create_image_views();
            create_render_pass();
            create_graphics_command_pool();
            create_primary_command_buffers();
            create_framebuffers();
            create_fence();
            create_semaphores();
        } catch (...) {
            teardown();
            throw;
        }
    }

    void render_context::init_swap_chain()
    {
        try {
            // called after construction, as otherwise the shared_from_this()
            // call would fail
            m_swap_chain = std::make_shared<mge::vulkan::swap_chain>(*this);
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
        mge::index_buffer_ref result =
            std::make_shared<index_buffer>(*this, dt, data_size, data);
        return result;
    }

    mge::vertex_buffer_ref render_context::create_vertex_buffer(
        const vertex_layout& layout, size_t data_size, void* data)
    {
        mge::vertex_buffer_ref result =
            std::make_shared<vertex_buffer>(*this, layout, data_size, data);
        return result;
    }

    mge::shader_ref render_context::create_shader(shader_type t)
    {
        mge::shader_ref result = std::make_shared<shader>(*this, t);
        return result;
    }

    mge::program_ref render_context::create_program()
    {
        mge::program_ref result = std::make_shared<program>(*this);
        return result;
    }

    mge::command_list_ref render_context::create_command_list()
    {
        mge::command_list_ref result =
            std::make_shared<mge::vulkan::command_list>(*this);
        return result;
    }

    mge::frame_command_list_ref
    render_context::create_current_frame_command_list()
    {
        if (!m_drawing_initialized) {
            initialize_drawing();
        }
        if (m_current_frame_state == frame_state::BEFORE_DRAW) {
            begin_frame();
            begin_draw();
        } else if (m_current_frame_state != frame_state::DRAW) {
            MGE_THROW(error)
                << "Invalid frame state for frame command list creation: "
                << m_current_frame_state;
        }
        auto result =
            std::make_shared<frame_command_list>(*this,
                                                 m_frame,
                                                 m_current_image_index);
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
        if (!m_render_system->vkCreateWin32SurfaceKHR) {
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
        CHECK_VK_CALL(m_render_system->vkCreateWin32SurfaceKHR(
            m_render_system->instance(),
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
            m_render_system->graphics_queue_index();
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
        if (m_render_system->debug()) {
            device_layers.push_back("VK_LAYER_KHRONOS_validation");
        }
        device_create_info.ppEnabledLayerNames = device_layers.data();
        device_create_info.enabledLayerCount =
            static_cast<uint32_t>(device_layers.size());
        device_create_info.enabledExtensionCount =
            static_cast<uint32_t>(device_extensions.size());
        device_create_info.ppEnabledExtensionNames = device_extensions.data();

        CHECK_VK_CALL(
            m_render_system->vkCreateDevice(m_render_system->physical_device(),
                                            &device_create_info,
                                            nullptr,
                                            &m_device));
        MGE_DEBUG_TRACE(VULKAN) << "Created logical device: " << m_device;
    }

    void render_context::teardown()
    {
        if (vkDestroySemaphore) {
            if (m_image_available_semaphore) {
                vkDestroySemaphore(m_device,
                                   m_image_available_semaphore,
                                   nullptr);
                m_image_available_semaphore = VK_NULL_HANDLE;
            }
            if (m_render_finished_semaphore) {
                vkDestroySemaphore(m_device,
                                   m_render_finished_semaphore,
                                   nullptr);
                m_render_finished_semaphore = VK_NULL_HANDLE;
            }
        }

        if (vkDestroyFence && m_frame_finished_fence) {
            vkDestroyFence(m_device, m_frame_finished_fence, nullptr);
            m_frame_finished_fence = VK_NULL_HANDLE;
        }

        if (vkDestroyFramebuffer) {
            for (auto fb : m_swap_chain_framebuffers) {
                if (fb != VK_NULL_HANDLE) {
                    vkDestroyFramebuffer(m_device, fb, nullptr);
                }
            }
            m_swap_chain_framebuffers.clear();
        }

        if (vkFreeCommandBuffers && m_primary_command_buffers.size() > 0) {
            vkFreeCommandBuffers(
                m_device,
                m_graphics_command_pool,
                mge::checked_cast<uint32_t>(m_primary_command_buffers.size()),
                m_primary_command_buffers.data());
            m_primary_command_buffers.clear();
        }

        if (vkDestroyCommandPool && m_graphics_command_pool) {
            vkDestroyCommandPool(m_device, m_graphics_command_pool, nullptr);
            m_graphics_command_pool = VK_NULL_HANDLE;
        }

        if (vkDestroyRenderPass && m_render_pass) {
            vkDestroyRenderPass(m_device, m_render_pass, nullptr);
            m_render_pass = VK_NULL_HANDLE;
        }

        if (vkDestroyImageView) {
            for (auto view : m_swap_chain_image_views) {
                if (view != VK_NULL_HANDLE) {
                    vkDestroyImageView(m_device, view, nullptr);
                }
            }
        }
        m_swap_chain_image_views.clear();

        m_swap_chain_images.clear();

        if (m_swap_chain_khr != VK_NULL_HANDLE && vkDestroySwapchainKHR) {
            vkDestroySwapchainKHR(m_device, m_swap_chain_khr, nullptr);
            m_swap_chain_khr = VK_NULL_HANDLE;
        }

        m_surface_capabilities = {};
        m_surface_formats.clear();
        m_surface_present_modes.clear();

        m_queue = VK_NULL_HANDLE;

        if (m_allocator) {
            vmaDestroyAllocator(m_allocator);
            m_allocator = VK_NULL_HANDLE;
        }

        if (m_device && vkDestroyDevice) {
            vkDestroyDevice(m_device, nullptr);
            m_device = VK_NULL_HANDLE;
        }

        clear_functions();

        if (m_surface != VK_NULL_HANDLE &&
            m_render_system->vkDestroySurfaceKHR) {
            m_render_system->vkDestroySurfaceKHR(m_render_system->instance(),
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
            // Too much output
            // MGE_DEBUG_TRACE(VULKAN)
            //    << "Replace " << name << ": " << (void*)(original) << " by "
            //    << (void*)ptr;
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
        resolve_device_function((void*)m_render_system->X,                     \
                                m_render_system->vkGetDeviceProcAddr,          \
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
                         m_render_system->graphics_queue_index(),
                         0,
                         &m_queue);
    }

    void render_context::fetch_surface_capabilities()
    {
        MGE_DEBUG_TRACE(VULKAN) << "Fetch surface capabilities";
        CHECK_VK_CALL(
            m_render_system->vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
                m_render_system->physical_device(),
                m_surface,
                &m_surface_capabilities));
        enumerate(
            [this](uint32_t* count, VkSurfaceFormatKHR* data) {
                CHECK_VK_CALL(
                    m_render_system->vkGetPhysicalDeviceSurfaceFormatsKHR(
                        m_render_system->physical_device(),
                        m_surface,
                        count,
                        data));
            },
            m_surface_formats);
        MGE_DEBUG_TRACE(VULKAN)
            << "Found " << m_surface_formats.size() << " surface formats";
        size_t format_index = m_surface_formats.size();
        for (const auto& format : m_surface_formats) {
            MGE_DEBUG_TRACE(VULKAN)
                << "    " << format.format << "/" << format.colorSpace;
            if (format.format == VK_FORMAT_B8G8R8A8_SRGB &&
                format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                format_index = &format - m_surface_formats.data();
            }
        }

        if (format_index < m_surface_formats.size()) {
            m_used_surface_format = m_surface_formats[format_index];
        } else {
            m_used_surface_format = m_surface_formats[0];
        }

        MGE_DEBUG_TRACE(VULKAN)
            << "Using surface format " << m_used_surface_format.format << "/"
            << m_used_surface_format.colorSpace;

        enumerate(
            [this](uint32_t* count, VkPresentModeKHR* data) {
                CHECK_VK_CALL(
                    m_render_system->vkGetPhysicalDeviceSurfacePresentModesKHR(
                        m_render_system->physical_device(),
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

        m_used_present_mode = VK_PRESENT_MODE_FIFO_KHR;
        for (const auto& mode : m_surface_present_modes) {
            if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                m_used_present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
                break;
            }
        }

        MGE_DEBUG_TRACE(VULKAN) << "Using present mode " << m_used_present_mode;
    }

    void render_context::choose_extent()
    {
        MGE_DEBUG_TRACE(VULKAN) << "Choose extent";
        if (m_surface_capabilities.currentExtent.width != UINT32_MAX) {
            m_extent = m_surface_capabilities.currentExtent;
        } else {
            m_extent.width =
                std::max(m_surface_capabilities.minImageExtent.width,
                         std::min(m_surface_capabilities.maxImageExtent.width,
                                  m_window.extent().width));
            m_extent.height =
                std::max(m_surface_capabilities.minImageExtent.height,
                         std::min(m_surface_capabilities.maxImageExtent.height,
                                  m_window.extent().height));
        }
        MGE_DEBUG_TRACE(VULKAN)
            << "Using extent " << m_extent.width << "x" << m_extent.height;
    }

    void render_context::create_swap_chain()
    {
        MGE_DEBUG_TRACE(VULKAN) << "Create swap chain";
        uint32_t image_count = m_surface_capabilities.minImageCount + 1;
        if (m_surface_capabilities.maxImageCount > 0 &&
            image_count > m_surface_capabilities.maxImageCount) {
            image_count = m_surface_capabilities.maxImageCount;
        }
        MGE_DEBUG_TRACE(VULKAN) << "Using " << image_count << " images";

        VkSwapchainCreateInfoKHR create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        create_info.surface = m_surface;
        create_info.minImageCount = image_count;
        create_info.imageFormat = m_used_surface_format.format;
        create_info.imageColorSpace = m_used_surface_format.colorSpace;
        create_info.imageExtent = m_extent;
        create_info.imageArrayLayers = 1;
        create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        uint32_t queue_indices[] = {m_render_system->graphics_queue_index(),
                                    m_render_system->present_queue_index()};

        if (m_render_system->graphics_queue_index() !=
            m_render_system->present_queue_index()) {
            create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            create_info.queueFamilyIndexCount = 2;
            create_info.pQueueFamilyIndices = queue_indices;
        } else {
            create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        create_info.preTransform = m_surface_capabilities.currentTransform;
        create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        create_info.presentMode = m_used_present_mode;
        create_info.clipped = VK_TRUE;
        create_info.oldSwapchain = VK_NULL_HANDLE;
        CHECK_VK_CALL(vkCreateSwapchainKHR(m_device,
                                           &create_info,
                                           nullptr,
                                           &m_swap_chain_khr));

        enumerate(
            [this](uint32_t* count, VkImage* data) {
                CHECK_VK_CALL(vkGetSwapchainImagesKHR(m_device,
                                                      m_swap_chain_khr,
                                                      count,
                                                      data));
            },
            m_swap_chain_images);
        MGE_DEBUG_TRACE(VULKAN) << "Created swap chain with "
                                << m_swap_chain_images.size() << " images";
    }

    void render_context::create_image_views()
    {
        MGE_DEBUG_TRACE(VULKAN) << "Create image views";
        m_swap_chain_image_views.resize(m_swap_chain_images.size());
        for (size_t i = 0; i < m_swap_chain_images.size(); ++i) {
            VkImageViewCreateInfo create_info = {};
            create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            create_info.image = m_swap_chain_images[i];
            create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            create_info.format = m_used_surface_format.format;
            create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            create_info.subresourceRange.baseMipLevel = 0;
            create_info.subresourceRange.levelCount = 1;
            create_info.subresourceRange.baseArrayLayer = 0;
            create_info.subresourceRange.layerCount = 1;
            CHECK_VK_CALL(vkCreateImageView(m_device,
                                            &create_info,
                                            nullptr,
                                            &m_swap_chain_image_views[i]));
        }
    }

    void render_context::create_allocator()
    {
        MGE_DEBUG_TRACE(VULKAN) << "Create allocator";
        VmaVulkanFunctions vk_functions = {};
        vk_functions.vkGetInstanceProcAddr =
            m_render_system->library().vkGetInstanceProcAddr;
        vk_functions.vkGetDeviceProcAddr = m_render_system->vkGetDeviceProcAddr;

        VmaAllocatorCreateInfo allocator_info = {};
        allocator_info.physicalDevice = m_render_system->physical_device();
        allocator_info.device = m_device;
        allocator_info.instance = m_render_system->instance();
        allocator_info.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
        allocator_info.pVulkanFunctions = &vk_functions;
        allocator_info.vulkanApiVersion = VK_API_VERSION_1_3;
        CHECK_VK_CALL(vmaCreateAllocator(&allocator_info, &m_allocator));
    }

    void render_context::create_render_pass()
    {
        MGE_DEBUG_TRACE(VULKAN) << "Create render pass";
        VkAttachmentDescription color_attachment = {};
        // single color buffer used for presentation
        color_attachment.format = m_used_surface_format.format;
        // TODO: multisampling in vulkan
        color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        // don't care about content of the image at beginning
        color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        // store content of the image for later
        color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        // nothing needed for stencil
        color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

        color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference color_attachment_ref = {};
        color_attachment_ref.attachment = 0;
        color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_attachment_ref;

        VkSubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo render_pass_info = {};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        render_pass_info.attachmentCount = 1;
        render_pass_info.pAttachments = &color_attachment;
        render_pass_info.subpassCount = 1;
        render_pass_info.pSubpasses = &subpass;
        render_pass_info.dependencyCount = 1;
        render_pass_info.pDependencies = &dependency;

        CHECK_VK_CALL(vkCreateRenderPass(m_device,
                                         &render_pass_info,
                                         nullptr,
                                         &m_render_pass));
    }

    void render_context::create_graphics_command_pool()
    {
        MGE_DEBUG_TRACE(VULKAN) << "Create graphics command pool";
        VkCommandPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        pool_info.queueFamilyIndex = m_render_system->graphics_queue_index();
        pool_info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT |
                          VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        CHECK_VK_CALL(vkCreateCommandPool(m_device,
                                          &pool_info,
                                          nullptr,
                                          &m_graphics_command_pool));
    }

    void render_context::create_primary_command_buffers()
    {
        MGE_DEBUG_TRACE(VULKAN) << "Create " << m_swap_chain_images.size()
                                << " primary command buffers";
        VkCommandBufferAllocateInfo alloc_info = {};
        alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        alloc_info.commandPool = m_graphics_command_pool;
        alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        alloc_info.commandBufferCount =
            mge::checked_cast<uint32_t>(m_swap_chain_images.size());
        m_primary_command_buffers.resize(m_swap_chain_images.size());
        CHECK_VK_CALL(
            vkAllocateCommandBuffers(m_device,
                                     &alloc_info,
                                     m_primary_command_buffers.data()));
        for (auto& cb : m_primary_command_buffers) {
            CHECK_VK_CALL(vkResetCommandBuffer(cb, 0));
        }
    }

    void render_context::create_framebuffers()
    {
        MGE_DEBUG_TRACE(VULKAN) << "Create framebuffers";
        m_swap_chain_framebuffers.resize(m_swap_chain_image_views.size());
        for (size_t i = 0; i < m_swap_chain_image_views.size(); ++i) {
            VkImageView attachments[] = {m_swap_chain_image_views[i]};

            VkFramebufferCreateInfo framebuffer_info = {};
            framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebuffer_info.renderPass = m_render_pass;
            framebuffer_info.attachmentCount = 1;
            framebuffer_info.pAttachments = attachments;
            framebuffer_info.width = m_extent.width;
            framebuffer_info.height = m_extent.height;
            framebuffer_info.layers = 1;

            CHECK_VK_CALL(vkCreateFramebuffer(m_device,
                                              &framebuffer_info,
                                              nullptr,
                                              &m_swap_chain_framebuffers[i]));
        }
    }

    void render_context::create_fence()
    {
        MGE_DEBUG_TRACE(VULKAN) << "Create fence";
        VkFenceCreateInfo fence_info = {};
        fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        // enable first draw begin to pass through
        fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        CHECK_VK_CALL(vkCreateFence(m_device,
                                    &fence_info,
                                    nullptr,
                                    &m_frame_finished_fence));
    }

    void render_context::create_semaphores()
    {
        MGE_DEBUG_TRACE(VULKAN) << "Create semaphores";
        VkSemaphoreCreateInfo semaphore_info = {};
        semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        CHECK_VK_CALL(vkCreateSemaphore(m_device,
                                        &semaphore_info,
                                        nullptr,
                                        &m_image_available_semaphore));
        CHECK_VK_CALL(vkCreateSemaphore(m_device,
                                        &semaphore_info,
                                        nullptr,
                                        &m_render_finished_semaphore));
    }

    void render_context::wait_for_frame_finished()
    {
        if (m_deleted_command_buffers.size() + m_deleted_pipelines.size() >=
            5) {
            gc();
        };
        // wait for finish of last frame
        CHECK_VK_CALL(vkWaitForFences(m_device,
                                      1,
                                      &m_frame_finished_fence,
                                      VK_TRUE,
                                      std::numeric_limits<uint64_t>::max()));
        ++m_frame;
        // reset fence
        CHECK_VK_CALL(vkResetFences(m_device, 1, &m_frame_finished_fence));
    }

    void render_context::acquire_next_image()
    {
        // acquire next swap chain image
        CHECK_VK_CALL(
            vkAcquireNextImageKHR(m_device,
                                  m_swap_chain_khr,
                                  std::numeric_limits<uint64_t>::max(),
                                  m_image_available_semaphore,
                                  VK_NULL_HANDLE,
                                  &m_current_image_index));
    }

    void render_context::begin_frame()
    {
        wait_for_frame_finished();
        acquire_next_image();
    }

    void render_context::begin_draw()
    {
        if (m_current_frame_state != frame_state::BEFORE_DRAW) {
            MGE_THROW(error) << "Invalid frame state for start of drawing: "
                             << m_current_frame_state;
        }

        CHECK_VK_CALL(
            vkResetCommandBuffer(current_primary_command_buffer(), 0));

        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = 0;
        begin_info.pInheritanceInfo = nullptr;
        CHECK_VK_CALL(vkBeginCommandBuffer(current_primary_command_buffer(),
                                           &begin_info));

        VkRenderPassBeginInfo render_pass_info{};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_info.renderPass = m_render_pass;
        render_pass_info.framebuffer =
            m_swap_chain_framebuffers[m_current_image_index];

        render_pass_info.renderArea.offset = {0, 0};
        render_pass_info.renderArea.extent = m_extent;

        VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
        render_pass_info.clearValueCount = 1;
        render_pass_info.pClearValues = &clear_color;

        vkCmdBeginRenderPass(current_primary_command_buffer(),
                             &render_pass_info,
                             VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
        m_current_frame_state = frame_state::DRAW;
        // MGE_DEBUG_TRACE(VULKAN) << "Begin draw";
    }

    void
    render_context::execute_frame_command_buffer(VkCommandBuffer command_buffer)
    {
        if (m_current_frame_state != frame_state::DRAW) {
            MGE_THROW(error)
                << "Invalid frame state for execution of frame command buffer: "
                << m_current_frame_state;
        }
        m_pending_command_buffers.push_back(command_buffer);
    }

    void render_context::end_draw()
    {
        if (m_pending_command_buffers.size() > 0) {
            // MGE_DEBUG_TRACE(VULKAN)
            //    << "Execute " << m_pending_command_buffers.size()
            //    << " secondary command buffers";
            vkCmdExecuteCommands(
                current_primary_command_buffer(),
                mge::checked_cast<uint32_t>(m_pending_command_buffers.size()),
                m_pending_command_buffers.data());
        }
        //  MGE_DEBUG_TRACE(VULKAN) << "End render pass";
        vkCmdEndRenderPass(current_primary_command_buffer());
        CHECK_VK_CALL(vkEndCommandBuffer(current_primary_command_buffer()));

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        VkSemaphore     wait_semaphores[] = {m_image_available_semaphore};
        VkSemaphore     signal_semaphores[] = {m_render_finished_semaphore};
        VkCommandBuffer command_buffers[] = {current_primary_command_buffer()};
        VkPipelineStageFlags wait_stages[] = {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = wait_semaphores;
        submit_info.pWaitDstStageMask = wait_stages;
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = signal_semaphores;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = command_buffers;

        CHECK_VK_CALL(
            vkQueueSubmit(m_queue, 1, &submit_info, m_frame_finished_fence));
        m_current_frame_state = frame_state::DRAW_FINISHED;
        m_pending_command_buffers.clear();
    }
#if 0
    void render_context::tmp_draw_all()
    {
        wait_for_frame_finished();
        acquire_next_image();

        CHECK_VK_CALL(vkResetCommandBuffer(m_tmp_command_buffer, 0));

        // begin command buffer recording
        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = 0;
        begin_info.pInheritanceInfo = nullptr;
        CHECK_VK_CALL(vkBeginCommandBuffer(m_tmp_command_buffer, &begin_info));

        VkRenderPassBeginInfo render_pass_info{};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_info.renderPass = m_render_pass;
        render_pass_info.framebuffer =
            m_swap_chain_framebuffers[m_current_image_index];

        render_pass_info.renderArea.offset = {0, 0};
        render_pass_info.renderArea.extent = m_extent;

        VkClearValue clear_color = {{{0.0f, 0.0f, 1.0f, 1.0f}}};
        render_pass_info.clearValueCount = 1;
        render_pass_info.pClearValues = &clear_color;

        vkCmdBeginRenderPass(m_tmp_command_buffer,
                             &render_pass_info,
                             VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(m_extent.width);
        viewport.height = static_cast<float>(m_extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(m_tmp_command_buffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = m_extent;
        vkCmdSetScissor(m_tmp_command_buffer, 0, 1, &scissor);

        // pipeline
        // draw

        vkCmdEndRenderPass(m_tmp_command_buffer);
        CHECK_VK_CALL(vkEndCommandBuffer(m_tmp_command_buffer));

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        VkSemaphore wait_semaphores[] = {m_image_available_semaphore};
        VkSemaphore signal_semaphores[] = {m_render_finished_semaphore};
        VkPipelineStageFlags wait_stages[] = {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = wait_semaphores;
        submit_info.pWaitDstStageMask = wait_stages;
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = signal_semaphores;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &m_tmp_command_buffer;

        CHECK_VK_CALL(
            vkQueueSubmit(m_queue, 1, &submit_info, m_frame_finished_fence));

        VkPresentInfoKHR present_info = {};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = signal_semaphores;
        present_info.swapchainCount = 1;
        present_info.pSwapchains = &m_swap_chain_khr;
        present_info.pImageIndices = &m_current_image_index;
        CHECK_VK_CALL(vkQueuePresentKHR(m_queue, &present_info));
    }
#endif

    void render_context::initialize_drawing()
    {
        // draw one frame to boot up the acquire/release cycle
        wait_for_frame_finished();
        acquire_next_image();

        VkCommandBuffer             tmp_command_buffer;
        VkCommandBufferAllocateInfo alloc_info = {};
        alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        alloc_info.commandPool = m_graphics_command_pool;
        alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        alloc_info.commandBufferCount = 1;

        CHECK_VK_CALL(vkAllocateCommandBuffers(m_device,
                                               &alloc_info,
                                               &tmp_command_buffer));

        CHECK_VK_CALL(vkResetCommandBuffer(tmp_command_buffer, 0));

        // begin command buffer recording
        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = 0;
        begin_info.pInheritanceInfo = nullptr;
        CHECK_VK_CALL(vkBeginCommandBuffer(tmp_command_buffer, &begin_info));

        VkRenderPassBeginInfo render_pass_info{};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_info.renderPass = m_render_pass;
        render_pass_info.framebuffer =
            m_swap_chain_framebuffers[m_current_image_index];

        render_pass_info.renderArea.offset = {0, 0};
        render_pass_info.renderArea.extent = m_extent;

        VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
        render_pass_info.clearValueCount = 1;
        render_pass_info.pClearValues = &clear_color;

        vkCmdBeginRenderPass(tmp_command_buffer,
                             &render_pass_info,
                             VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(m_extent.width);
        viewport.height = static_cast<float>(m_extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(tmp_command_buffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = m_extent;
        vkCmdSetScissor(tmp_command_buffer, 0, 1, &scissor);

        vkCmdEndRenderPass(tmp_command_buffer);
        CHECK_VK_CALL(vkEndCommandBuffer(tmp_command_buffer));

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        VkSemaphore wait_semaphores[] = {m_image_available_semaphore};
        VkSemaphore signal_semaphores[] = {m_render_finished_semaphore};
        VkPipelineStageFlags wait_stages[] = {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = wait_semaphores;
        submit_info.pWaitDstStageMask = wait_stages;
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = signal_semaphores;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &tmp_command_buffer;

        CHECK_VK_CALL(
            vkQueueSubmit(m_queue, 1, &submit_info, m_frame_finished_fence));

        VkPresentInfoKHR present_info = {};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = signal_semaphores;
        present_info.swapchainCount = 1;
        present_info.pSwapchains = &m_swap_chain_khr;
        present_info.pImageIndices = &m_current_image_index;
        CHECK_VK_CALL(vkQueuePresentKHR(m_queue, &present_info));

        // schedule command buffer for gc
        m_deleted_command_buffers.emplace_back(0, tmp_command_buffer);

        m_drawing_initialized = true;
    }

    void render_context::present()
    {
        switch (m_current_frame_state) {
        case frame_state::BEFORE_DRAW:
            begin_frame();
            [[fallthrough]];
        case frame_state::DRAW:
            end_draw();
            [[fallthrough]];
        case frame_state::DRAW_FINISHED:
            break;
        }

        VkSemaphore      wait_semaphores[] = {m_render_finished_semaphore};
        VkPresentInfoKHR present_info = {};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = wait_semaphores;
        present_info.swapchainCount = 1;
        present_info.pSwapchains = &m_swap_chain_khr;
        present_info.pImageIndices = &m_current_image_index;
        CHECK_VK_CALL(vkQueuePresentKHR(m_queue, &present_info));

        m_current_frame_state = frame_state::BEFORE_DRAW;
    }

    void render_context::discard_command_buffer(uint64_t        frame,
                                                VkCommandBuffer command_buffer)
    {
        if (frame < m_frame) {
            destroy_command_buffer(command_buffer);
        } else {
            m_deleted_command_buffers.emplace_back(frame, command_buffer);
        }
    }

    void render_context::discard_pipeline(uint64_t frame, VkPipeline pipeline)
    {
        if (frame < m_frame) {
            destroy_pipeline(pipeline);
        } else {
            m_deleted_pipelines.emplace_back(frame, pipeline);
        }
    }

    void render_context::destroy_pipeline(VkPipeline pipeline)
    {
        vkDestroyPipeline(m_device, pipeline, nullptr);
    }

    void render_context::destroy_command_buffer(VkCommandBuffer command_buffer)
    {
        vkFreeCommandBuffers(m_device,
                             m_graphics_command_pool,
                             1,
                             &command_buffer);
    }

    void render_context::gc()
    {
        std::vector<std::pair<uint64_t, VkCommandBuffer>> new_deleted;
        for (auto& cb : m_deleted_command_buffers) {
            if (cb.first < m_frame) {
                destroy_command_buffer(cb.second);
            } else {
                new_deleted.push_back(cb);
            }
        }
        m_deleted_command_buffers.swap(new_deleted);
    }

} // namespace mge::vulkan