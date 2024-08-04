// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "swap_chain.hpp"
#include "enumerate.hpp"
#include "error.hpp"
#include "mge/core/trace.hpp"
#include "render_context.hpp"
#include "render_system.hpp"

namespace mge {
    MGE_USE_TRACE(VULKAN);
}

namespace mge::vulkan {

    swap_chain::swap_chain(render_context& context)
        : mge::swap_chain(context)
        , m_vulkan_context(context)
        , m_swap_chain(VK_NULL_HANDLE)
        , m_image_available(VK_NULL_HANDLE)
        , m_current_image(std::numeric_limits<uint32_t>::max())
    {
        try {
            create_swap_chain(context);
            get_images(context);
            create_image_views(context);
            create_semaphore(context);
        } catch (...) {
            cleanup();
            throw;
        }
    }

    void swap_chain::create_semaphore(render_context& context)
    {
        VkSemaphoreCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        CHECK_VK_CALL(context.vkCreateSemaphore(context.device(),
                                                &create_info,
                                                nullptr,
                                                &m_image_available));
    }

    void swap_chain::create_swap_chain(render_context& context)
    {
        VkSwapchainCreateInfoKHR create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        create_info.surface = context.surface();
        create_info.minImageCount = context.default_image_count();
        auto surface_format = context.format();

        create_info.imageFormat = surface_format.format;
        create_info.imageColorSpace = surface_format.colorSpace;
        create_info.imageExtent = context.extent();
        create_info.imageArrayLayers = 1;
        create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32_t queue_families[] = {
            context.render_system().graphics_queue_family_index(),
            context.present_queue_family_index()};
        // different graphics and present queue
        if (queue_families[0] != queue_families[1]) {
            create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            create_info.queueFamilyIndexCount = 2;
            create_info.pQueueFamilyIndices = queue_families;
        } else {
            create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        create_info.preTransform = context.capabilities().currentTransform;
        create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        create_info.presentMode = context.present_mode();
        create_info.clipped = VK_TRUE;
        create_info.oldSwapchain = VK_NULL_HANDLE;

        CHECK_VK_CALL(context.vkCreateSwapchainKHR(context.device(),
                                                   &create_info,
                                                   nullptr,
                                                   &m_swap_chain));
    }

    void swap_chain::get_images(render_context& context)
    {

        enumerate(
            [this, &context](uint32_t* count, VkImage* data) {
                CHECK_VK_CALL(context.vkGetSwapchainImagesKHR(context.device(),
                                                              m_swap_chain,
                                                              count,
                                                              data));
            },
            m_images);
    }

    void swap_chain::create_image_views(render_context& context)
    {
        m_image_views.resize(m_images.size(), VK_NULL_HANDLE);
        for (size_t i = 0; i < m_images.size(); ++i) {
            VkImageViewCreateInfo create_info = {};
            create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            create_info.image = m_images[i];
            create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            create_info.format = context.format().format;
            create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            create_info.subresourceRange.baseMipLevel = 0;
            create_info.subresourceRange.levelCount = 1;
            create_info.subresourceRange.baseArrayLayer = 0;
            create_info.subresourceRange.layerCount = 1;
            CHECK_VK_CALL(context.vkCreateImageView(context.device(),
                                                    &create_info,
                                                    nullptr,
                                                    &m_image_views[i]));
        }
    }

    void swap_chain::cleanup()
    {
        auto& context = m_vulkan_context;
        if (m_image_available) {
            context.vkDestroySemaphore(context.device(),
                                       m_image_available,
                                       nullptr);
            m_image_available = VK_NULL_HANDLE;
        }

        for (const auto& fb : m_frame_buffers) {
            context.vkDestroyFramebuffer(context.device(), fb, nullptr);
        }
        m_frame_buffers.clear();

        for (const auto& iv : m_image_views) {
            context.vkDestroyImageView(context.device(), iv, nullptr);
        }
        m_image_views.clear();

        m_images.clear(); // owned by swap chain, no destroy

        if (m_swap_chain) {
            context.vkDestroySwapchainKHR(context.device(),
                                          m_swap_chain,
                                          nullptr);
            m_swap_chain = VK_NULL_HANDLE;
        }
    }

    void swap_chain::create_frame_buffers(VkRenderPass render_pass)
    {
        auto& context = m_vulkan_context;

        m_frame_buffers.resize(m_image_views.size(), VK_NULL_HANDLE);

        for (size_t i = 0; i < m_image_views.size(); ++i) {
            VkImageView attachments[] = {m_image_views[i]};

            VkFramebufferCreateInfo create_info{};
            create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            create_info.renderPass = render_pass;
            create_info.attachmentCount = 1;
            create_info.pAttachments = attachments;
            create_info.width = context.extent().width;
            create_info.height = context.extent().height;
            create_info.layers = 1;

            CHECK_VK_CALL(context.vkCreateFramebuffer(context.device(),
                                                      &create_info,
                                                      nullptr,
                                                      &m_frame_buffers[i]));
        }
    }

    swap_chain::~swap_chain() { cleanup(); }

    void swap_chain::present()
    {
        m_vulkan_context.end_draw();

        VkPresentInfoKHR present_info{};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        VkSemaphore wait_semaphores[] = {m_vulkan_context.render_finished()};
        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = &wait_semaphores[0];

        present_info.swapchainCount = 1;
        present_info.pSwapchains = &m_swap_chain;
        present_info.pImageIndices = &m_current_image;

        CHECK_VK_CALL(
            m_vulkan_context.vkQueuePresentKHR(m_vulkan_context.present_queue(),
                                               &present_info));

        m_vulkan_context.wait_for_frame_completion();
        // advance to next frame
        next_frame();
    }

    VkFramebuffer swap_chain::current_frame()
    {
        return m_frame_buffers[m_current_image];
    }

    VkFramebuffer swap_chain::next_frame()
    {
        CHECK_VK_CALL(m_vulkan_context.vkAcquireNextImageKHR(
            m_vulkan_context.device(),
            m_swap_chain,
            std::numeric_limits<uint64_t>::max(),
            m_image_available,
            VK_NULL_HANDLE,
            &m_current_image));
        return m_frame_buffers[m_current_image];
    }

} // namespace mge::vulkan