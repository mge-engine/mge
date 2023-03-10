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
        , m_swap_chain(VK_NULL_HANDLE)
    {
        create_swap_chain(context);
        get_images(context);
        create_image_views(context);
    }

    render_context& swap_chain::vulkan_context()
    {
        return dynamic_cast<render_context&>(context());
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

        CHECK_VK_CALL(context.render_system().vkCreateSwapchainKHR(
            context.render_system().device(),
            &create_info,
            nullptr,
            &m_swap_chain));
    }

    void swap_chain::get_images(render_context& context)
    {

        enumerate(
            [this, &context](uint32_t* count, VkImage* data) {
                CHECK_VK_CALL(context.render_system().vkGetSwapchainImagesKHR(
                    context.render_system().device(),
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
            CHECK_VK_CALL(context.render_system().vkCreateImageView(
                context.render_system().device(),
                &create_info,
                nullptr,
                &m_image_views[i]));
        }
    }

    void swap_chain::cleanup()
    {
        auto& context = vulkan_context();

        for (const auto& fb : m_frame_buffers) {
            context.render_system().vkDestroyFramebuffer(
                context.render_system().device(),
                fb,
                nullptr);
        }
        m_frame_buffers.clear();

        for (const auto& iv : m_image_views) {
            context.render_system().vkDestroyImageView(
                context.render_system().device(),
                iv,
                nullptr);
        }
        m_image_views.clear();

        m_images.clear(); // owned by swap chain, no destroy

        context.render_system().vkDestroySwapchainKHR(
            context.render_system().device(),
            m_swap_chain,
            nullptr);
        m_swap_chain = VK_NULL_HANDLE;
    }

    void swap_chain::create_frame_buffers(VkRenderPass render_pass)
    {
        auto& context = vulkan_context();

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

            CHECK_VK_CALL(context.render_system().vkCreateFramebuffer(
                context.render_system().device(),
                &create_info,
                nullptr,
                &m_frame_buffers[i]));
        }
    }

    swap_chain::~swap_chain() { cleanup(); }

    void swap_chain::present() {}
} // namespace mge::vulkan