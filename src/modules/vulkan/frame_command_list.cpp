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
        alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
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
    }

    void frame_command_list::clear(const mge::rgba_color& c) {}

    void frame_command_list::draw(const mge::draw_command& command)
    {
        // MGE_THROW_NOT_IMPLEMENTED;
    }

    void frame_command_list::execute()
    { // MGE_THROW_NOT_IMPLEMENTED;
    }

    void frame_command_list::finish()
    { // MGE_THROW_NOT_IMPLEMENTED;
    }
} // namespace mge::vulkan