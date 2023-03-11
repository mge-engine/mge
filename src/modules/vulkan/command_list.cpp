// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "command_list.hpp"
#include "error.hpp"
#include "render_context.hpp"
#include "render_system.hpp"

namespace mge::vulkan {

    command_list::command_list(render_context& context)
        : mge::command_list(context, true)
        , m_vulkan_context(context)
        , m_command_buffer(VK_NULL_HANDLE)
    {
        allocate_command_buffer(context);
    }

    command_list::~command_list() { cleanup(); }

    void command_list::clear(const mge::rgba_color& color)
    {
        m_clear_color = color;
    }

    void command_list::draw(const mge::draw_command& command) {}

    void command_list::finish() {}

    void command_list::execute() { m_vulkan_context.execute_on_frame(this); }

    void command_list::allocate_command_buffer(render_context& context)

    {
        VkCommandBufferAllocateInfo alloc_info{};
        alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        alloc_info.commandPool = context.command_pool();
        alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        alloc_info.commandBufferCount = 1;

        CHECK_VK_CALL(context.vkAllocateCommandBuffers(context.device(),
                                                       &alloc_info,
                                                       &m_command_buffer));
    }

    void command_list::cleanup()
    {
        auto& ctx = m_vulkan_context;
        if (m_command_buffer) {
            ctx.vkFreeCommandBuffers(ctx.device(),
                                     ctx.command_pool(),
                                     1,
                                     &m_command_buffer);
            m_command_buffer = VK_NULL_HANDLE;
        }
    }

    void command_list::record_on_frame(VkFramebuffer frame_buffer)
    {
        auto& ctx = m_vulkan_context;
        CHECK_VK_CALL(ctx.vkResetCommandBuffer(m_command_buffer, 0));

        VkCommandBufferBeginInfo begin_info{};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        CHECK_VK_CALL(ctx.vkBeginCommandBuffer(m_command_buffer, &begin_info));

        VkRenderPassBeginInfo render_pass_info{};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_info.renderPass = ctx.render_pass();
        render_pass_info.framebuffer = frame_buffer;
        render_pass_info.renderArea.offset = {0, 0};
        render_pass_info.renderArea.extent.width = ctx.extent().width;
        render_pass_info.renderArea.extent.height = ctx.extent().height;

        VkClearValue clear_color = {};
        clear_color.color.float32[0] = m_clear_color.r;
        clear_color.color.float32[1] = m_clear_color.g;
        clear_color.color.float32[2] = m_clear_color.b;
        clear_color.color.float32[3] = m_clear_color.a;
        render_pass_info.clearValueCount = 1;
        render_pass_info.pClearValues = &clear_color;

        ctx.vkCmdBeginRenderPass(m_command_buffer,
                                 &render_pass_info,
                                 VK_SUBPASS_CONTENTS_INLINE);

        ctx.vkCmdEndRenderPass(m_command_buffer);

        CHECK_VK_CALL(ctx.vkEndCommandBuffer(m_command_buffer));
    }

} // namespace mge::vulkan