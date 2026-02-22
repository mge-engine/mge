// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "texture.hpp"
#include "error.hpp"
#include "render_context.hpp"

#include "mge/core/checked_cast.hpp"
#include "mge/core/trace.hpp"

namespace mge {
    MGE_USE_TRACE(VULKAN);
}

namespace mge::vulkan {

    texture::texture(render_context& context, mge::texture_type type)
        : mge::texture(context, type)
    {}

    texture::~texture()
    {
        auto& ctx = static_cast<render_context&>(context());

        if (m_sampler != VK_NULL_HANDLE) {
            ctx.vkDestroySampler(ctx.device(), m_sampler, nullptr);
        }
        if (m_image_view != VK_NULL_HANDLE) {
            ctx.vkDestroyImageView(ctx.device(), m_image_view, nullptr);
        }
        if (m_image != VK_NULL_HANDLE) {
            vmaDestroyImage(ctx.allocator(), m_image, m_allocation);
        }
    }

    VkFormat texture::texture_format(const mge::image_format& format) const
    {
        switch (format.format()) {
        case mge::image_format::data_format::RGB:
            switch (format.type()) {
            case mge::data_type::FLOAT:
                return VK_FORMAT_R32G32B32_SFLOAT;
            default:
                MGE_THROW(mge::illegal_argument)
                    << "Unsupported image format (data type): " << format;
            }
        case mge::image_format::data_format::RGBA:
            switch (format.type()) {
            case mge::data_type::UINT8:
                return VK_FORMAT_R8G8B8A8_UNORM;
            case mge::data_type::UINT16:
                return VK_FORMAT_R16G16B16A16_UNORM;
            case mge::data_type::FLOAT:
                return VK_FORMAT_R32G32B32A32_SFLOAT;
            default:
                MGE_THROW(mge::illegal_argument)
                    << "Unsupported image format (data type): " << format;
            }
        default:
            MGE_THROW(mge::illegal_argument)
                << "Unsupported image format (format): " << format;
        }
    }

    void texture::create_image(VkFormat format, uint32_t width, uint32_t height)
    {
        auto& ctx = static_cast<render_context&>(context());

        VkImageCreateInfo image_info = {};
        image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        image_info.imageType = VK_IMAGE_TYPE_2D;
        image_info.extent.width = width;
        image_info.extent.height = height;
        image_info.extent.depth = 1;
        image_info.mipLevels = 1;
        image_info.arrayLayers = 1;
        image_info.format = format;
        image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
        image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        image_info.usage =
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        image_info.samples = VK_SAMPLE_COUNT_1_BIT;
        image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo alloc_info = {};
        alloc_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;

        CHECK_VK_CALL(vmaCreateImage(ctx.allocator(),
                                     &image_info,
                                     &alloc_info,
                                     &m_image,
                                     &m_allocation,
                                     nullptr));
    }

    void texture::create_image_view(VkFormat format)
    {
        auto& ctx = static_cast<render_context&>(context());

        VkImageViewCreateInfo view_info = {};
        view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        view_info.image = m_image;
        view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        view_info.format = format;
        view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        view_info.subresourceRange.baseMipLevel = 0;
        view_info.subresourceRange.levelCount = 1;
        view_info.subresourceRange.baseArrayLayer = 0;
        view_info.subresourceRange.layerCount = 1;

        CHECK_VK_CALL(ctx.vkCreateImageView(ctx.device(),
                                            &view_info,
                                            nullptr,
                                            &m_image_view));
    }

    void texture::create_sampler()
    {
        auto& ctx = static_cast<render_context&>(context());

        VkSamplerCreateInfo sampler_info = {};
        sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        sampler_info.magFilter = VK_FILTER_LINEAR;
        sampler_info.minFilter = VK_FILTER_LINEAR;
        sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        sampler_info.anisotropyEnable = VK_FALSE;
        sampler_info.maxAnisotropy = 1.0f;
        sampler_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        sampler_info.unnormalizedCoordinates = VK_FALSE;
        sampler_info.compareEnable = VK_FALSE;
        sampler_info.compareOp = VK_COMPARE_OP_ALWAYS;
        sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        sampler_info.mipLodBias = 0.0f;
        sampler_info.minLod = 0.0f;
        sampler_info.maxLod = 0.0f;

        CHECK_VK_CALL(ctx.vkCreateSampler(ctx.device(),
                                          &sampler_info,
                                          nullptr,
                                          &m_sampler));
    }

    void texture::transition_image_layout(VkCommandBuffer command_buffer,
                                          VkImageLayout   old_layout,
                                          VkImageLayout   new_layout)
    {
        VkImageMemoryBarrier barrier = {};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = old_layout;
        barrier.newLayout = new_layout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = m_image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags src_stage;
        VkPipelineStageFlags dst_stage;

        if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED &&
            new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            src_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        } else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
                   new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            src_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            dst_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        } else {
            MGE_THROW(mge::illegal_argument) << "Unsupported layout transition";
        }

        auto& ctx = static_cast<render_context&>(context());
        ctx.vkCmdPipelineBarrier(command_buffer,
                                 src_stage,
                                 dst_stage,
                                 0,
                                 0,
                                 nullptr,
                                 0,
                                 nullptr,
                                 1,
                                 &barrier);
    }

    void texture::upload_data(const void* data,
                              size_t      size,
                              uint32_t    width,
                              uint32_t    height,
                              size_t      row_pitch)
    {
        auto& ctx = static_cast<render_context&>(context());

        // Create staging buffer
        VkBufferCreateInfo buffer_info = {};
        buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buffer_info.size = size;
        buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo alloc_info = {};
        alloc_info.usage = VMA_MEMORY_USAGE_CPU_ONLY;

        VkBuffer      staging_buffer = VK_NULL_HANDLE;
        VmaAllocation staging_allocation = VK_NULL_HANDLE;

        CHECK_VK_CALL(vmaCreateBuffer(ctx.allocator(),
                                      &buffer_info,
                                      &alloc_info,
                                      &staging_buffer,
                                      &staging_allocation,
                                      nullptr));

        // Copy data to staging buffer
        void* mapped = nullptr;
        CHECK_VK_CALL(
            vmaMapMemory(ctx.allocator(), staging_allocation, &mapped));
        memcpy(mapped, data, size);
        vmaUnmapMemory(ctx.allocator(), staging_allocation);

        // Allocate a one-shot command buffer
        VkCommandBufferAllocateInfo cmd_alloc_info = {};
        cmd_alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        cmd_alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        cmd_alloc_info.commandPool = ctx.graphics_command_pool();
        cmd_alloc_info.commandBufferCount = 1;

        VkCommandBuffer command_buffer = VK_NULL_HANDLE;
        CHECK_VK_CALL(ctx.vkAllocateCommandBuffers(ctx.device(),
                                                   &cmd_alloc_info,
                                                   &command_buffer));

        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        CHECK_VK_CALL(ctx.vkBeginCommandBuffer(command_buffer, &begin_info));

        // Transition to transfer destination
        transition_image_layout(command_buffer,
                                VK_IMAGE_LAYOUT_UNDEFINED,
                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        // Copy buffer to image
        VkBufferImageCopy region = {};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = {0, 0, 0};
        region.imageExtent = {width, height, 1};

        ctx.vkCmdCopyBufferToImage(command_buffer,
                                   staging_buffer,
                                   m_image,
                                   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                   1,
                                   &region);

        // Transition to shader read-only
        transition_image_layout(command_buffer,
                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        CHECK_VK_CALL(ctx.vkEndCommandBuffer(command_buffer));

        // Submit and wait
        VkSubmitInfo submit_info = {};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &command_buffer;

        CHECK_VK_CALL(ctx.vkQueueSubmit(ctx.graphics_queue(),
                                        1,
                                        &submit_info,
                                        VK_NULL_HANDLE));
        CHECK_VK_CALL(ctx.vkQueueWaitIdle(ctx.graphics_queue()));

        ctx.vkFreeCommandBuffers(ctx.device(),
                                 ctx.graphics_command_pool(),
                                 1,
                                 &command_buffer);

        // Destroy staging buffer
        vmaDestroyBuffer(ctx.allocator(), staging_buffer, staging_allocation);
    }

    void texture::set_data(const mge::image_format& format,
                           const mge::extent&       extent,
                           const void*              data,
                           size_t                   size)
    {
        MGE_DEBUG_TRACE(VULKAN,
                        "Set texture data: {}x{}, format {}",
                        extent.width,
                        extent.height,
                        format);

        VkFormat vk_format = texture_format(format);
        size_t   row_pitch = extent.width * format.binary_size();

        create_image(vk_format, extent.width, extent.height);
        upload_data(data, size, extent.width, extent.height, row_pitch);
        create_image_view(vk_format);
        create_sampler();
    }

} // namespace mge::vulkan
