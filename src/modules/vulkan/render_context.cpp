// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "render_context.hpp"
#include "enumerate.hpp"
#include "error.hpp"
#include "frame_buffer.hpp"
#include "render_system.hpp"
#include "texture.hpp"
#include "window.hpp"

#include "mge/core/checked_cast.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/frame_debugger.hpp"
#include "mge/graphics/memory_image.hpp"

#if defined(MGE_OS_MACOSX) || defined(MGE_OS_LINUX)
#    include <GLFW/glfw3.h>
#endif

namespace mge {
    MGE_USE_TRACE(VULKAN);
}

namespace mge::vulkan {

    render_context::render_context(mge::vulkan::render_system& render_system_,
                                   window&                     window_)
        : render_context_base(render_system_, window_.extent())
        , m_window(window_)
    {
        try {
            create_surface();
            create_device();
            resolve_device_functions();
            create_allocator();
            get_device_queue();
            init_capabilities();
            fetch_surface_capabilities();
            choose_extent();
            mge::render_context::m_extent = {m_extent.width, m_extent.height};
            create_swap_chain();
            create_image_views();
            find_depth_format();
            create_depth_resources();
            create_render_pass();
            create_graphics_command_pool();
            create_primary_command_buffers();
            create_framebuffers();
            create_fence();
            create_semaphores();
            create_descriptor_pool();

            auto fd = m_render_system->frame_debugger();
            if (fd) {
                MGE_INFO_TRACE(
                    VULKAN,
                    "Note: For Vulkan, launch application through RenderDoc "
                    "to enable frame capture");
            }
        } catch (...) {
            teardown();
            throw;
        }
    }

    render_context::~render_context()
    {
        if (m_render_system->frame_debugger()) {
            auto fd = m_render_system->frame_debugger();
            if (fd) {
                MGE_INFO_TRACE(VULKAN, "Ending frame recording");
                fd->end_capture();
            }
        }

        if (m_device) {
            vkDeviceWaitIdle(m_device);
        }
        teardown();
    }

    std::vector<const char*> render_context::get_device_extensions() const
    {
        std::vector<const char*> extensions;
        extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        return extensions;
    }

    void render_context::create_surface()
    {
#ifdef MGE_OS_WINDOWS
        MGE_DEBUG_TRACE(VULKAN, "Create Vulkan surface (Win32)");
        if (!m_render_system->vkCreateWin32SurfaceKHR) {
            MGE_THROW(error)
                << "Cannot create surface: vkCreateWin32SurfaceKHR function "
                   "missing";
        }
        VkWin32SurfaceCreateInfoKHR create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        create_info.hinstance = GetModuleHandle(nullptr);
        create_info.hwnd = m_window.hwnd();
        create_info.flags = 0;
        CHECK_VK_CALL(m_render_system->vkCreateWin32SurfaceKHR(
            m_render_system->instance(),
            &create_info,
            nullptr,
            &m_surface));
#elif defined(MGE_OS_MACOSX) || defined(MGE_OS_LINUX)
        MGE_DEBUG_TRACE(VULKAN, "Create Vulkan surface (GLFW)");
        CHECK_VK_CALL(glfwCreateWindowSurface(m_render_system->instance(),
                                              m_window.handle(),
                                              nullptr,
                                              &m_surface));
#else
#    error Missing port
#endif
    }

    void render_context::teardown()
    {
        // Clean up uniform buffers and descriptor pool (shared)
        // These are cleaned up in teardown_shared, but we must clean
        // windowed-specific resources first.

        if (vkDestroySemaphore) {
            for (auto s : m_image_available_semaphores) {
                if (s != VK_NULL_HANDLE) {
                    vkDestroySemaphore(m_device, s, nullptr);
                }
            }
            m_image_available_semaphores.clear();
            for (auto s : m_render_finished_semaphores) {
                if (s != VK_NULL_HANDLE) {
                    vkDestroySemaphore(m_device, s, nullptr);
                }
            }
            m_render_finished_semaphores.clear();
        }

        if (vkDestroyFence) {
            for (auto f : m_frame_finished_fences) {
                if (f != VK_NULL_HANDLE) {
                    vkDestroyFence(m_device, f, nullptr);
                }
            }
            m_frame_finished_fences.clear();
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

        if (vkDestroyImageView) {
            for (auto view : m_depth_image_views) {
                if (view != VK_NULL_HANDLE) {
                    vkDestroyImageView(m_device, view, nullptr);
                }
            }
        }
        m_depth_image_views.clear();

        for (size_t i = 0; i < m_depth_images.size(); ++i) {
            if (m_allocator && m_depth_images[i] != VK_NULL_HANDLE) {
                vmaDestroyImage(m_allocator,
                                m_depth_images[i],
                                m_depth_image_allocations[i]);
            }
        }
        m_depth_images.clear();
        m_depth_image_allocations.clear();

        m_swap_chain_images.clear();

        if (m_swap_chain_khr != VK_NULL_HANDLE && vkDestroySwapchainKHR) {
            vkDestroySwapchainKHR(m_device, m_swap_chain_khr, nullptr);
            m_swap_chain_khr = VK_NULL_HANDLE;
        }

        m_surface_capabilities = {};
        m_surface_formats.clear();
        m_surface_present_modes.clear();

        teardown_shared();

        if (m_surface != VK_NULL_HANDLE &&
            m_render_system->vkDestroySurfaceKHR) {
            m_render_system->vkDestroySurfaceKHR(m_render_system->instance(),
                                                 m_surface,
                                                 nullptr);
            m_surface = VK_NULL_HANDLE;
        }
    }

    void render_context::fetch_surface_capabilities()
    {
        MGE_DEBUG_TRACE(VULKAN, "Fetch surface capabilities");
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
        MGE_DEBUG_TRACE(VULKAN,
                        "Found {} surface formats",
                        m_surface_formats.size());
        size_t format_index = m_surface_formats.size();
        for (const auto& format : m_surface_formats) {
            MGE_DEBUG_TRACE(VULKAN,
                            "    {} / {}",
                            format.format,
                            format.colorSpace);
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

        MGE_DEBUG_TRACE(VULKAN,
                        "Using surface format {} / {}",
                        m_used_surface_format.format,
                        m_used_surface_format.colorSpace);

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

        MGE_DEBUG_TRACE(VULKAN,
                        "Found {} present modes",
                        m_surface_present_modes.size());
        for (const auto& f : m_surface_present_modes) {
            MGE_DEBUG_TRACE(VULKAN, "    {}", f);
        }

        m_used_present_mode = VK_PRESENT_MODE_FIFO_KHR;
        for (const auto& mode : m_surface_present_modes) {
            if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                m_used_present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
                break;
            }
        }

        MGE_DEBUG_TRACE(VULKAN, "Using present mode {}", m_used_present_mode);
    }

    void render_context::choose_extent()
    {
        MGE_DEBUG_TRACE(VULKAN, "Choose extent");
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
        MGE_DEBUG_TRACE(VULKAN,
                        "Using extent {}x{}",
                        m_extent.width,
                        m_extent.height);
    }

    void render_context::create_swap_chain()
    {
        MGE_DEBUG_TRACE(VULKAN, "Create swap chain");
        uint32_t image_count = m_surface_capabilities.minImageCount + 1;
        if (m_surface_capabilities.maxImageCount > 0 &&
            image_count > m_surface_capabilities.maxImageCount) {
            image_count = m_surface_capabilities.maxImageCount;
        }
        MGE_DEBUG_TRACE(VULKAN, "Using {} images", image_count);

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
        MGE_DEBUG_TRACE(VULKAN,
                        "Created swap chain with {} images",
                        m_swap_chain_images.size());
    }

    void render_context::create_image_views()
    {
        MGE_DEBUG_TRACE(VULKAN, "Create image views");
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

    void render_context::create_depth_resources()
    {
        MGE_DEBUG_TRACE(VULKAN, "Create depth resources");

        m_depth_images.resize(m_swap_chain_images.size());
        m_depth_image_allocations.resize(m_swap_chain_images.size());
        m_depth_image_views.resize(m_swap_chain_images.size());

        for (size_t i = 0; i < m_swap_chain_images.size(); ++i) {
            VkImageCreateInfo image_info = {};
            image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            image_info.imageType = VK_IMAGE_TYPE_2D;
            image_info.extent.width = m_extent.width;
            image_info.extent.height = m_extent.height;
            image_info.extent.depth = 1;
            image_info.mipLevels = 1;
            image_info.arrayLayers = 1;
            image_info.format = m_depth_format;
            image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
            image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            image_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            image_info.samples = VK_SAMPLE_COUNT_1_BIT;
            image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            VmaAllocationCreateInfo alloc_info = {};
            alloc_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;

            CHECK_VK_CALL(vmaCreateImage(m_allocator,
                                         &image_info,
                                         &alloc_info,
                                         &m_depth_images[i],
                                         &m_depth_image_allocations[i],
                                         nullptr));

            VkImageViewCreateInfo view_info = {};
            view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            view_info.image = m_depth_images[i];
            view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            view_info.format = m_depth_format;
            view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            view_info.subresourceRange.baseMipLevel = 0;
            view_info.subresourceRange.levelCount = 1;
            view_info.subresourceRange.baseArrayLayer = 0;
            view_info.subresourceRange.layerCount = 1;

            CHECK_VK_CALL(vkCreateImageView(m_device,
                                            &view_info,
                                            nullptr,
                                            &m_depth_image_views[i]));
        }
    }

    void render_context::create_render_pass()
    {
        MGE_DEBUG_TRACE(VULKAN, "Create render pass");
        VkAttachmentDescription color_attachment = {};
        color_attachment.format = m_used_surface_format.format;
        color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentDescription depth_attachment = {};
        depth_attachment.format = m_depth_format;
        depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depth_attachment.finalLayout =
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference color_attachment_ref = {};
        color_attachment_ref.attachment = 0;
        color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depth_attachment_ref = {};
        depth_attachment_ref.attachment = 1;
        depth_attachment_ref.layout =
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_attachment_ref;
        subpass.pDepthStencilAttachment = &depth_attachment_ref;

        VkSubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask =
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask =
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                                   VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        VkAttachmentDescription attachments[] = {color_attachment,
                                                 depth_attachment};

        VkRenderPassCreateInfo render_pass_info = {};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        render_pass_info.attachmentCount = 2;
        render_pass_info.pAttachments = attachments;
        render_pass_info.subpassCount = 1;
        render_pass_info.pSubpasses = &subpass;
        render_pass_info.dependencyCount = 1;
        render_pass_info.pDependencies = &dependency;

        CHECK_VK_CALL(vkCreateRenderPass(m_device,
                                         &render_pass_info,
                                         nullptr,
                                         &m_render_pass));
    }

    void render_context::create_primary_command_buffers()
    {
        MGE_DEBUG_TRACE(VULKAN,
                        "Create {} primary command buffers",
                        m_swap_chain_images.size());
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
        MGE_DEBUG_TRACE(VULKAN, "Create framebuffers");
        m_swap_chain_framebuffers.resize(m_swap_chain_image_views.size());
        for (size_t i = 0; i < m_swap_chain_image_views.size(); ++i) {
            VkImageView attachments[] = {m_swap_chain_image_views[i],
                                         m_depth_image_views[i]};

            VkFramebufferCreateInfo framebuffer_info = {};
            framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebuffer_info.renderPass = m_render_pass;
            framebuffer_info.attachmentCount = 2;
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
        MGE_DEBUG_TRACE(VULKAN, "Create fences");
        m_frame_finished_fences.resize(m_swap_chain_images.size());
        VkFenceCreateInfo fence_info = {};
        fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        for (size_t i = 0; i < m_swap_chain_images.size(); ++i) {
            CHECK_VK_CALL(vkCreateFence(m_device,
                                        &fence_info,
                                        nullptr,
                                        &m_frame_finished_fences[i]));
        }
    }

    void render_context::create_semaphores()
    {
        MGE_DEBUG_TRACE(VULKAN, "Create semaphores");
        m_image_available_semaphores.resize(m_swap_chain_images.size());
        m_render_finished_semaphores.resize(m_swap_chain_images.size());
        VkSemaphoreCreateInfo semaphore_info = {};
        semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        for (size_t i = 0; i < m_swap_chain_images.size(); ++i) {
            CHECK_VK_CALL(vkCreateSemaphore(m_device,
                                            &semaphore_info,
                                            nullptr,
                                            &m_image_available_semaphores[i]));
            CHECK_VK_CALL(vkCreateSemaphore(m_device,
                                            &semaphore_info,
                                            nullptr,
                                            &m_render_finished_semaphores[i]));
        }
    }

    void render_context::wait_for_frame_finished()
    {
        CHECK_VK_CALL(vkWaitForFences(m_device,
                                      1,
                                      &m_frame_finished_fences[m_current_frame],
                                      VK_TRUE,
                                      std::numeric_limits<uint64_t>::max()));
        ++m_frame;
        CHECK_VK_CALL(vkResetFences(m_device,
                                    1,
                                    &m_frame_finished_fences[m_current_frame]));
    }

    void render_context::acquire_next_image()
    {
        CHECK_VK_CALL(
            vkAcquireNextImageKHR(m_device,
                                  m_swap_chain_khr,
                                  std::numeric_limits<uint64_t>::max(),
                                  m_image_available_semaphores[m_current_frame],
                                  VK_NULL_HANDLE,
                                  &m_current_image_index));
    }

    void render_context::render(const mge::pass& p)
    {
        if (m_current_frame_state == frame_state::BEFORE_DRAW) {
            wait_for_frame_finished();
            acquire_next_image();
            auto cmd = current_primary_command_buffer();
            CHECK_VK_CALL(vkResetCommandBuffer(cmd, 0));
            VkCommandBufferBeginInfo begin_info = {};
            begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            CHECK_VK_CALL(vkBeginCommandBuffer(cmd, &begin_info));
            m_current_frame_state = frame_state::DRAW;
        }

        VkCommandBuffer command_buffer = current_primary_command_buffer();

        VkRenderPass  pass_render_pass;
        VkFramebuffer pass_framebuffer;
        VkExtent2D    pass_extent;

        if (!p.frame_buffer()) {
            pass_render_pass = m_render_pass;
            pass_framebuffer = m_swap_chain_framebuffers[m_current_image_index];
            pass_extent      = m_extent;
        } else {
            auto* vk_fb = static_cast<mge::vulkan::frame_buffer*>(
                p.frame_buffer().get());
            pass_render_pass = vk_fb->render_pass();
            pass_framebuffer = vk_fb->vk_framebuffer();
            pass_extent      = vk_fb->fbo_extent();
        }

        record_render_pass(p,
                           pass_render_pass,
                           pass_framebuffer,
                           pass_extent,
                           command_buffer);
    }

    void render_context::on_frame_present()
    {
        CHECK_VK_CALL(vkEndCommandBuffer(current_primary_command_buffer()));
        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        VkSemaphore wait_semaphores[] = {
            m_image_available_semaphores[m_current_frame]};
        VkSemaphore signal_semaphores[] = {
            m_render_finished_semaphores[m_current_frame]};
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

        CHECK_VK_CALL(vkQueueSubmit(m_queue,
                                    1,
                                    &submit_info,
                                    m_frame_finished_fences[m_current_frame]));
        m_current_frame_state = frame_state::DRAW_FINISHED;

        VkSemaphore present_wait_semaphores[] = {
            m_render_finished_semaphores[m_current_frame]};
        VkPresentInfoKHR present_info = {};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = present_wait_semaphores;
        present_info.swapchainCount = 1;
        present_info.pSwapchains = &m_swap_chain_khr;
        present_info.pImageIndices = &m_current_image_index;
        CHECK_VK_CALL(vkQueuePresentKHR(m_queue, &present_info));
        m_current_frame_state = frame_state::BEFORE_DRAW;
        m_current_frame = (m_current_frame + 1) %
                          static_cast<uint32_t>(m_swap_chain_images.size());
    }

    mge::image_ref render_context::screenshot()
    {
        VkImage      swap_image = m_swap_chain_images[m_current_image_index];
        uint32_t     w = m_extent.width;
        uint32_t     h = m_extent.height;
        VkDeviceSize image_size = static_cast<VkDeviceSize>(w) * h * 4;

        VkBufferCreateInfo buffer_info = {};
        buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buffer_info.size = image_size;
        buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo alloc_info = {};
        alloc_info.usage = VMA_MEMORY_USAGE_GPU_TO_CPU;
        alloc_info.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

        VkBuffer          staging_buffer = VK_NULL_HANDLE;
        VmaAllocation     staging_alloc = VK_NULL_HANDLE;
        VmaAllocationInfo staging_alloc_info = {};
        CHECK_VK_CALL(vmaCreateBuffer(m_allocator,
                                      &buffer_info,
                                      &alloc_info,
                                      &staging_buffer,
                                      &staging_alloc,
                                      &staging_alloc_info));

        VkCommandBufferAllocateInfo cmd_alloc_info = {};
        cmd_alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        cmd_alloc_info.commandPool = m_graphics_command_pool;
        cmd_alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        cmd_alloc_info.commandBufferCount = 1;

        VkCommandBuffer cmd = VK_NULL_HANDLE;
        CHECK_VK_CALL(
            vkAllocateCommandBuffers(m_device, &cmd_alloc_info, &cmd));

        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        CHECK_VK_CALL(vkBeginCommandBuffer(cmd, &begin_info));

        VkImageMemoryBarrier to_transfer = {};
        to_transfer.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        to_transfer.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        to_transfer.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        to_transfer.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        to_transfer.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        to_transfer.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        to_transfer.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        to_transfer.image = swap_image;
        to_transfer.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        to_transfer.subresourceRange.levelCount = 1;
        to_transfer.subresourceRange.layerCount = 1;
        vkCmdPipelineBarrier(cmd,
                             VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                             VK_PIPELINE_STAGE_TRANSFER_BIT,
                             0,
                             0,
                             nullptr,
                             0,
                             nullptr,
                             1,
                             &to_transfer);

        VkBufferImageCopy region = {};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = {0, 0, 0};
        region.imageExtent = {w, h, 1};
        vkCmdCopyImageToBuffer(cmd,
                               swap_image,
                               VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                               staging_buffer,
                               1,
                               &region);

        VkImageMemoryBarrier to_present = {};
        to_present.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        to_present.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        to_present.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        to_present.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        to_present.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        to_present.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        to_present.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        to_present.image = swap_image;
        to_present.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        to_present.subresourceRange.levelCount = 1;
        to_present.subresourceRange.layerCount = 1;
        vkCmdPipelineBarrier(cmd,
                             VK_PIPELINE_STAGE_TRANSFER_BIT,
                             VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                             0,
                             0,
                             nullptr,
                             0,
                             nullptr,
                             1,
                             &to_present);

        CHECK_VK_CALL(vkEndCommandBuffer(cmd));

        VkFenceCreateInfo fence_info = {};
        fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        VkFence fence = VK_NULL_HANDLE;
        CHECK_VK_CALL(vkCreateFence(m_device, &fence_info, nullptr, &fence));

        VkSubmitInfo submit_info = {};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &cmd;
        CHECK_VK_CALL(vkQueueSubmit(m_queue, 1, &submit_info, fence));
        CHECK_VK_CALL(
            vkWaitForFences(m_device, 1, &fence, VK_TRUE, UINT64_MAX));

        mge::image_format fmt(mge::image_format::data_format::RGBA,
                              mge::data_type::UINT8);
        auto img = std::make_shared<mge::memory_image>(fmt, mge::extent(w, h));

        auto* dst = static_cast<uint8_t*>(img->data());
        auto* src = static_cast<const uint8_t*>(staging_alloc_info.pMappedData);
        uint32_t row_size = w * 4;

        bool need_swizzle =
            m_used_surface_format.format == VK_FORMAT_B8G8R8A8_UNORM ||
            m_used_surface_format.format == VK_FORMAT_B8G8R8A8_SRGB;

        for (uint32_t y = 0; y < h; ++y) {
            memcpy(dst + y * row_size, src + y * row_size, row_size);
            if (need_swizzle) {
                uint8_t* row = dst + y * row_size;
                for (uint32_t x = 0; x < w; ++x) {
                    std::swap(row[x * 4], row[x * 4 + 2]);
                }
            }
        }

        vkDestroyFence(m_device, fence, nullptr);
        vkFreeCommandBuffers(m_device, m_graphics_command_pool, 1, &cmd);
        vmaDestroyBuffer(m_allocator, staging_buffer, staging_alloc);

        return img;
    }

} // namespace mge::vulkan
