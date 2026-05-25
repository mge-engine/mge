// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "frame_buffer.hpp"
#include "error.hpp"
#include "render_context_base.hpp"
#include "texture.hpp"

#include "mge/core/trace.hpp"

namespace mge {
    MGE_USE_TRACE(VULKAN);
}

namespace mge::vulkan {

    static VkFormat color_format_to_vk(const mge::image_format& fmt)
    {
        switch (fmt.format()) {
        case mge::image_format::data_format::RGBA:
            switch (fmt.type()) {
            case mge::data_type::UINT8:
                return VK_FORMAT_R8G8B8A8_UNORM;
            case mge::data_type::UINT16:
                return VK_FORMAT_R16G16B16A16_UNORM;
            case mge::data_type::FLOAT:
                return VK_FORMAT_R32G32B32A32_SFLOAT;
            default:
                break;
            }
            break;
        case mge::image_format::data_format::RGB:
            if (fmt.type() == mge::data_type::FLOAT) {
                return VK_FORMAT_R32G32B32_SFLOAT;
            }
            break;
        default:
            break;
        }
        MGE_THROW(mge::illegal_argument)
            << "Unsupported color attachment format: " << fmt;
    }

    frame_buffer::frame_buffer(render_context_base&           ctx,
                                const mge::frame_buffer_info& info)
        : mge::frame_buffer(ctx)
        , m_context(ctx)
    {
        create_render_pass(info);

        uint32_t slot = 0;
        for (const auto& ca : info.color_attachments) {
            auto tex = ctx.create_render_target_texture(
                mge::texture_type::TYPE_2D, ca.format, ca.extent);
            if (m_extent.width == 0) {
                m_extent = {ca.extent.width, ca.extent.height};
            }
            attach_color(tex, slot++);
        }

        if (info.depth_stencil_extent) {
            mge::image_format depth_fmt(
                mge::image_format::data_format::DEPTH_STENCIL,
                mge::data_type::UINT32);
            auto dtex = ctx.create_render_target_texture(
                mge::texture_type::TYPE_2D,
                depth_fmt,
                *info.depth_stencil_extent);
            if (m_extent.width == 0) {
                m_extent = {info.depth_stencil_extent->width,
                            info.depth_stencil_extent->height};
            }
            attach_depth(dtex);
        }

        create_framebuffer();
    }

    frame_buffer::~frame_buffer()
    {
        if (m_framebuffer != VK_NULL_HANDLE) {
            m_context.vkDestroyFramebuffer(m_context.device(),
                                           m_framebuffer,
                                           nullptr);
        }
        if (m_render_pass != VK_NULL_HANDLE) {
            m_context.vkDestroyRenderPass(m_context.device(),
                                          m_render_pass,
                                          nullptr);
        }
    }

    void frame_buffer::on_attach_color(const mge::texture_ref& tex, uint32_t slot)
    {
        // RTV pre-created on the texture; nothing extra needed here
    }

    void frame_buffer::on_attach_depth(const mge::texture_ref& tex)
    {
        // DSV pre-created on the texture; nothing extra needed here
    }

    void frame_buffer::create_render_pass(const mge::frame_buffer_info& info)
    {
        std::vector<VkAttachmentDescription> attachments;
        std::vector<VkAttachmentReference>   color_refs;

        for (size_t i = 0; i < info.color_attachments.size(); ++i) {
            VkAttachmentDescription desc = {};
            desc.format  = color_format_to_vk(info.color_attachments[i].format);
            desc.samples = VK_SAMPLE_COUNT_1_BIT;
            desc.loadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            desc.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            desc.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
            desc.finalLayout    = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            attachments.push_back(desc);

            VkAttachmentReference ref = {};
            ref.attachment = static_cast<uint32_t>(i);
            ref.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            color_refs.push_back(ref);
        }

        bool                  has_depth = info.depth_stencil_extent.has_value();
        VkAttachmentReference depth_ref = {};

        if (has_depth) {
            VkAttachmentDescription depth_desc = {};
            depth_desc.format  = m_context.depth_format();
            depth_desc.samples = VK_SAMPLE_COUNT_1_BIT;
            depth_desc.loadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            depth_desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            depth_desc.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            depth_desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
            depth_desc.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
            depth_desc.finalLayout =
                VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            depth_ref.attachment = static_cast<uint32_t>(attachments.size());
            depth_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            attachments.push_back(depth_desc);
        }

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount =
            static_cast<uint32_t>(color_refs.size());
        subpass.pColorAttachments =
            color_refs.empty() ? nullptr : color_refs.data();
        subpass.pDepthStencilAttachment = has_depth ? &depth_ref : nullptr;

        VkSubpassDependency dependency = {};
        dependency.srcSubpass    = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass    = 0;
        dependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                                   VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                                   VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        if (has_depth) {
            dependency.dstAccessMask |=
                VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        }

        VkRenderPassCreateInfo rp_info = {};
        rp_info.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        rp_info.attachmentCount = static_cast<uint32_t>(attachments.size());
        rp_info.pAttachments    = attachments.data();
        rp_info.subpassCount    = 1;
        rp_info.pSubpasses      = &subpass;
        rp_info.dependencyCount = 1;
        rp_info.pDependencies   = &dependency;

        CHECK_VK_CALL(m_context.vkCreateRenderPass(m_context.device(),
                                                   &rp_info,
                                                   nullptr,
                                                   &m_render_pass));
    }

    void frame_buffer::create_framebuffer()
    {
        std::vector<VkImageView> views;

        for (uint32_t i = 0; i < static_cast<uint32_t>(m_color_attachments.size()); ++i) {
            auto* tex =
                static_cast<vulkan::texture*>(color_attachment(i).get());
            views.push_back(tex->image_view());
        }

        if (auto dtex = depth_attachment()) {
            auto* tex = static_cast<vulkan::texture*>(dtex.get());
            views.push_back(tex->image_view());
        }

        VkFramebufferCreateInfo fb_info = {};
        fb_info.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fb_info.renderPass      = m_render_pass;
        fb_info.attachmentCount = static_cast<uint32_t>(views.size());
        fb_info.pAttachments    = views.data();
        fb_info.width           = m_extent.width;
        fb_info.height          = m_extent.height;
        fb_info.layers          = 1;

        CHECK_VK_CALL(m_context.vkCreateFramebuffer(m_context.device(),
                                                    &fb_info,
                                                    nullptr,
                                                    &m_framebuffer));
    }

} // namespace mge::vulkan
