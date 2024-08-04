#include "frame_command_list.hpp"
#include "error.hpp"
#include "render_context.hpp"

namespace mge::vulkan {
    frame_command_list::frame_command_list(render_context& context,
                                           uint64_t        frame,
                                           uint32_t        backbuffer_index)
        : mge::frame_command_list(context, backbuffer_index, true)
        , m_vulkan_context(context)
        , m_frame(frame)
    {
        VkCommandBufferAllocateInfo alloc_info = {};
        alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        alloc_info.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
        alloc_info.commandPool = m_vulkan_context.graphics_command_pool();
        alloc_info.commandBufferCount = 1;

        CHECK_VK_CALL(
            m_vulkan_context.vkAllocateCommandBuffers(m_vulkan_context.device(),
                                                      &alloc_info,
                                                      &m_command_buffer));
        begin();
    }

    frame_command_list::~frame_command_list()
    {
        if (m_command_buffer) {
            m_vulkan_context.discard_command_buffer(m_frame, m_command_buffer);
            m_command_buffer = VK_NULL_HANDLE;
        }
    }

    void frame_command_list::begin()
    {
        // initially reset command buffer
        CHECK_VK_CALL(
            m_vulkan_context.vkResetCommandBuffer(m_command_buffer, 0));
        // begin, inheriting the render pass
        VkCommandBufferInheritanceInfo inheritance_info = {};
        inheritance_info.sType =
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
        inheritance_info.renderPass = m_vulkan_context.render_pass();
        inheritance_info.subpass = 0;
        inheritance_info.framebuffer =
            m_vulkan_context.framebuffer(backbuffer_index());
        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
        begin_info.pInheritanceInfo = &inheritance_info;
        CHECK_VK_CALL(m_vulkan_context.vkBeginCommandBuffer(m_command_buffer,
                                                            &begin_info));
    }

    void frame_command_list::viewport(const mge::viewport& vp)
    {
        const VkViewport* p_vk_cp =
            reinterpret_cast<const VkViewport*>(vp.data());
        m_vulkan_context.vkCmdSetViewport(m_command_buffer, 0, 1, p_vk_cp);
    }

    void frame_command_list::scissor(const mge::rectangle& r)
    {
        VkRect2D vk_cp = {};
        vk_cp.offset.x = static_cast<int32_t>(r.left);
        vk_cp.offset.y = static_cast<int32_t>(r.top);
        vk_cp.extent.width = static_cast<int32_t>(r.width());
        vk_cp.extent.height = static_cast<int32_t>(r.height());
        m_vulkan_context.vkCmdSetScissor(m_command_buffer, 0, 1, &vk_cp);
    }

    void frame_command_list::default_scissor()
    {
        VkRect2D vk_cp = {};
        vk_cp.offset = {0, 0};
        vk_cp.extent = m_vulkan_context.extent();
        m_vulkan_context.vkCmdSetScissor(m_command_buffer, 0, 1, &vk_cp);
    }

    void frame_command_list::clear(const mge::rgba_color& c)
    {
        VkClearValue clear_color = {};
        clear_color.color = {{c.r, c.g, c.b, c.a}};
        VkClearAttachment clear_attachment = {};
        clear_attachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        clear_attachment.colorAttachment = 0;
        clear_attachment.clearValue = clear_color;
        VkClearRect clear_rect = {};
        clear_rect.rect.offset = {0, 0};
        clear_rect.rect.extent = m_vulkan_context.extent();
        clear_rect.baseArrayLayer = 0;
        clear_rect.layerCount = 1;
        m_vulkan_context.vkCmdClearAttachments(m_command_buffer,
                                               1,
                                               &clear_attachment,
                                               1,
                                               &clear_rect);
    }

    void frame_command_list::draw(const mge::draw_command& command)
    {
        // MGE_THROW_NOT_IMPLEMENTED;
    }

    void frame_command_list::execute()
    {
        m_vulkan_context.execute_frame_command_buffer(m_command_buffer);
    }

    void frame_command_list::finish()
    {
        CHECK_VK_CALL(m_vulkan_context.vkEndCommandBuffer(m_command_buffer));
    }
} // namespace mge::vulkan