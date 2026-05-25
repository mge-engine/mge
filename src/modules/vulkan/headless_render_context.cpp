// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "headless_render_context.hpp"
#include "error.hpp"
#include "frame_buffer.hpp"
#include "render_system.hpp"
#include "texture.hpp"

#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/memory_image.hpp"

namespace mge {
    MGE_USE_TRACE(VULKAN);
}

namespace mge::vulkan {

    headless_render_context::headless_render_context(
        mge::vulkan::render_system& rs, const mge::extent& ext)
        : render_context_base(rs, ext)
    {
        try {
            create_device();
            resolve_device_functions();
            create_allocator();
            get_device_queue();
            init_capabilities();
            find_depth_format();
            create_graphics_command_pool();
            create_command_buffer();
            create_render_fence();
            create_descriptor_pool();
        } catch (...) {
            teardown();
            throw;
        }
    }

    headless_render_context::~headless_render_context()
    {
        if (m_device) {
            vkDeviceWaitIdle(m_device);
        }
        teardown();
    }

    std::vector<const char*>
    headless_render_context::get_device_extensions() const
    {
        return {};
    }

    void headless_render_context::create_command_buffer()
    {
        MGE_DEBUG_TRACE(VULKAN, "Create headless command buffer");
        VkCommandBufferAllocateInfo alloc_info = {};
        alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        alloc_info.commandPool = m_graphics_command_pool;
        alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        alloc_info.commandBufferCount = 1;
        CHECK_VK_CALL(
            vkAllocateCommandBuffers(m_device, &alloc_info, &m_command_buffer));
    }

    void headless_render_context::create_render_fence()
    {
        MGE_DEBUG_TRACE(VULKAN, "Create headless fence");
        VkFenceCreateInfo fence_info = {};
        fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        CHECK_VK_CALL(vkCreateFence(m_device, &fence_info, nullptr, &m_fence));
    }

    void headless_render_context::teardown()
    {
        if (m_fence != VK_NULL_HANDLE && vkDestroyFence) {
            vkDestroyFence(m_device, m_fence, nullptr);
            m_fence = VK_NULL_HANDLE;
        }
        if (m_command_buffer != VK_NULL_HANDLE && vkFreeCommandBuffers &&
            m_graphics_command_pool != VK_NULL_HANDLE) {
            vkFreeCommandBuffers(m_device,
                                 m_graphics_command_pool,
                                 1,
                                 &m_command_buffer);
            m_command_buffer = VK_NULL_HANDLE;
        }
        teardown_shared();
    }

    void headless_render_context::render(const mge::pass& p)
    {
        if (m_frame_state == frame_state::BEFORE_DRAW) {
            CHECK_VK_CALL(vkWaitForFences(m_device,
                                          1,
                                          &m_fence,
                                          VK_TRUE,
                                          std::numeric_limits<uint64_t>::max()));
            CHECK_VK_CALL(vkResetFences(m_device, 1, &m_fence));
            CHECK_VK_CALL(vkResetCommandBuffer(m_command_buffer, 0));
            VkCommandBufferBeginInfo begin_info = {};
            begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            CHECK_VK_CALL(vkBeginCommandBuffer(m_command_buffer, &begin_info));
            m_frame_state = frame_state::DRAW;
        }

        if (!p.frame_buffer()) {
            MGE_THROW(mge::illegal_state)
                << "Headless render context requires a frame buffer";
        }

        auto* vk_fb =
            static_cast<mge::vulkan::frame_buffer*>(p.frame_buffer().get());
        m_last_frame_buffer = vk_fb;

        record_render_pass(p,
                           vk_fb->render_pass(),
                           vk_fb->vk_framebuffer(),
                           vk_fb->fbo_extent(),
                           m_command_buffer);
    }

    void headless_render_context::on_frame_present()
    {
        CHECK_VK_CALL(vkEndCommandBuffer(m_command_buffer));

        VkSubmitInfo submit_info = {};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &m_command_buffer;

        CHECK_VK_CALL(vkQueueSubmit(m_queue, 1, &submit_info, m_fence));
        CHECK_VK_CALL(vkWaitForFences(m_device,
                                      1,
                                      &m_fence,
                                      VK_TRUE,
                                      std::numeric_limits<uint64_t>::max()));
        m_frame_state = frame_state::BEFORE_DRAW;
    }

    mge::image_ref headless_render_context::screenshot()
    {
        if (!m_last_frame_buffer) {
            MGE_THROW(mge::illegal_state)
                << "No frame buffer was rendered to";
        }

        auto* fb = static_cast<mge::vulkan::frame_buffer*>(m_last_frame_buffer);
        auto  color_ref = fb->color_attachment(0);
        if (!color_ref) {
            MGE_THROW(mge::illegal_state)
                << "Frame buffer has no color attachment";
        }
        auto* vk_tex = static_cast<mge::vulkan::texture*>(color_ref.get());
        VkImage src_image = vk_tex->vk_image();

        VkExtent2D ext = fb->fbo_extent();
        uint32_t   w = ext.width;
        uint32_t   h = ext.height;
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

        // Transition: SHADER_READ_ONLY -> TRANSFER_SRC
        VkImageMemoryBarrier to_transfer = {};
        to_transfer.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        to_transfer.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        to_transfer.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        to_transfer.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        to_transfer.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        to_transfer.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        to_transfer.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        to_transfer.image = src_image;
        to_transfer.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        to_transfer.subresourceRange.levelCount = 1;
        to_transfer.subresourceRange.layerCount = 1;
        vkCmdPipelineBarrier(cmd,
                             VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
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
                               src_image,
                               VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                               staging_buffer,
                               1,
                               &region);

        // Transition back: TRANSFER_SRC -> SHADER_READ_ONLY
        VkImageMemoryBarrier to_read = {};
        to_read.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        to_read.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        to_read.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        to_read.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        to_read.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        to_read.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        to_read.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        to_read.image = src_image;
        to_read.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        to_read.subresourceRange.levelCount = 1;
        to_read.subresourceRange.layerCount = 1;
        vkCmdPipelineBarrier(cmd,
                             VK_PIPELINE_STAGE_TRANSFER_BIT,
                             VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                             0,
                             0,
                             nullptr,
                             0,
                             nullptr,
                             1,
                             &to_read);

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

        vmaInvalidateAllocation(m_allocator, staging_alloc, 0, VK_WHOLE_SIZE);

        mge::image_format fmt(mge::image_format::data_format::RGBA,
                              mge::data_type::UINT8);
        auto img = std::make_shared<mge::memory_image>(fmt, mge::extent(w, h));

        auto*    dst = static_cast<uint8_t*>(img->data());
        auto*    src =
            static_cast<const uint8_t*>(staging_alloc_info.pMappedData);
        uint32_t row_size = w * 4;
        for (uint32_t y = 0; y < h; ++y) {
            memcpy(dst + y * row_size, src + y * row_size, row_size);
        }

        vkDestroyFence(m_device, fence, nullptr);
        vkFreeCommandBuffers(m_device, m_graphics_command_pool, 1, &cmd);
        vmaDestroyBuffer(m_allocator, staging_buffer, staging_alloc);

        return img;
    }

} // namespace mge::vulkan
