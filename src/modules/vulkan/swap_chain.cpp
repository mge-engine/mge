// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "swap_chain.hpp"
#include "error.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/graphics/image_format.hpp"
#include "mge/graphics/memory_image.hpp"

namespace mge::vulkan {

    swap_chain::swap_chain(render_context& context)
        : mge::swap_chain(context)
        , m_render_context(context)
    {}

    swap_chain::~swap_chain() {}

    void swap_chain::present()
    {
        m_render_context.present();
    }

    uint32_t swap_chain::current_back_buffer_index() const
    {
        return m_render_context.current_image_index();
    }

    image_ref swap_chain::screenshot()
    {
        auto extent = m_render_context.extent();

        // Get the current swap chain image
        uint32_t image_index = m_render_context.current_image_index();
        VkImage  swap_chain_image =
            m_render_context.swap_chain_image(image_index);

        // Create staging buffer for CPU access
        VkDeviceSize buffer_size = extent.width * extent.height * 4; // RGBA

        VkBufferCreateInfo buffer_info = {};
        buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buffer_info.size = buffer_size;
        buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo alloc_info = {};
        alloc_info.usage = VMA_MEMORY_USAGE_GPU_TO_CPU;
        alloc_info.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

        VkBuffer          staging_buffer;
        VmaAllocation     staging_allocation;
        VmaAllocationInfo staging_alloc_info;

        auto result = vmaCreateBuffer(m_render_context.allocator(),
                                      &buffer_info,
                                      &alloc_info,
                                      &staging_buffer,
                                      &staging_allocation,
                                      &staging_alloc_info);

        if (result != VK_SUCCESS) {
            MGE_THROW(mge::vulkan::error) << "Failed to create staging buffer";
        }

        // Create command buffer for the copy operation
        VkCommandBufferAllocateInfo cmd_alloc_info = {};
        cmd_alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        cmd_alloc_info.commandPool = m_render_context.graphics_command_pool();
        cmd_alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        cmd_alloc_info.commandBufferCount = 1;

        VkCommandBuffer command_buffer;
        result =
            m_render_context.vkAllocateCommandBuffers(m_render_context.device(),
                                                      &cmd_alloc_info,
                                                      &command_buffer);

        if (result != VK_SUCCESS) {
            vmaDestroyBuffer(m_render_context.allocator(),
                             staging_buffer,
                             staging_allocation);
            MGE_THROW(mge::vulkan::error)
                << "Failed to allocate command buffer";
        }

        // Begin command buffer
        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        m_render_context.vkBeginCommandBuffer(command_buffer, &begin_info);

        // Transition swap chain image to transfer source
        VkImageMemoryBarrier barrier = {};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = swap_chain_image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        m_render_context.vkCmdPipelineBarrier(command_buffer,
                                              VK_PIPELINE_STAGE_TRANSFER_BIT,
                                              VK_PIPELINE_STAGE_TRANSFER_BIT,
                                              0,
                                              0,
                                              nullptr,
                                              0,
                                              nullptr,
                                              1,
                                              &barrier);

        // Copy image to buffer
        VkBufferImageCopy region = {};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = {0, 0, 0};
        region.imageExtent = {extent.width, extent.height, 1};

        m_render_context.vkCmdCopyImageToBuffer(
            command_buffer,
            swap_chain_image,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            staging_buffer,
            1,
            &region);

        // Transition swap chain image back to present
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;

        m_render_context.vkCmdPipelineBarrier(command_buffer,
                                              VK_PIPELINE_STAGE_TRANSFER_BIT,
                                              VK_PIPELINE_STAGE_TRANSFER_BIT,
                                              0,
                                              0,
                                              nullptr,
                                              0,
                                              nullptr,
                                              1,
                                              &barrier);

        m_render_context.vkEndCommandBuffer(command_buffer);

        // Submit command buffer
        VkSubmitInfo submit_info = {};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &command_buffer;

        result =
            m_render_context.vkQueueSubmit(m_render_context.graphics_queue(),
                                           1,
                                           &submit_info,
                                           VK_NULL_HANDLE);

        if (result != VK_SUCCESS) {
            m_render_context.vkFreeCommandBuffers(
                m_render_context.device(),
                m_render_context.graphics_command_pool(),
                1,
                &command_buffer);
            vmaDestroyBuffer(m_render_context.allocator(),
                             staging_buffer,
                             staging_allocation);
            MGE_THROW(mge::vulkan::error) << "Failed to submit command buffer";
        }

        // Wait for operations to complete
        m_render_context.vkQueueWaitIdle(m_render_context.graphics_queue());

        // Free command buffer
        m_render_context.vkFreeCommandBuffers(
            m_render_context.device(),
            m_render_context.graphics_command_pool(),
            1,
            &command_buffer);

        // Map the staging buffer and copy data
        void* mapped_data = staging_alloc_info.pMappedData;

        // Create RGBA image
        image_format format(image_format::data_format::RGBA, data_type::UINT8);
        auto         img = std::make_shared<memory_image>(
            format,
            mge::extent(extent.width, extent.height));

        // Convert from BGRA to RGBA
        const uint8_t* src = static_cast<const uint8_t*>(mapped_data);
        uint8_t*       dst = static_cast<uint8_t*>(img->data());

        for (uint32_t i = 0; i < extent.width * extent.height; ++i) {
            dst[i * 4 + 0] = src[i * 4 + 2]; // R from B
            dst[i * 4 + 1] = src[i * 4 + 1]; // G from G
            dst[i * 4 + 2] = src[i * 4 + 0]; // B from R
            dst[i * 4 + 3] = src[i * 4 + 3]; // A from A
        }

        // Clean up
        vmaDestroyBuffer(m_render_context.allocator(),
                         staging_buffer,
                         staging_allocation);

        return img;
    }

} // namespace mge::vulkan