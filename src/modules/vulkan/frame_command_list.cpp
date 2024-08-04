#include "frame_command_list.hpp"
#include "error.hpp"
#include "render_context.hpp"

namespace mge::vulkan {
    frame_command_list::frame_command_list(render_context& context,
                                           uint32_t        backbuffer_index)
        : mge::frame_command_list(context, backbuffer_index, true)
        , m_vulkan_context(context)
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
        if (m_command_buffer && m_vulkan_context.device() &&
            m_vulkan_context.graphics_command_pool() &&
            m_vulkan_context.vkFreeCommandBuffers) {
            m_vulkan_context.vkFreeCommandBuffers(
                m_vulkan_context.device(),
                m_vulkan_context.graphics_command_pool(),
                1,
                &m_command_buffer);
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

    void frame_command_list::clear(const mge::rgba_color& c) {}

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