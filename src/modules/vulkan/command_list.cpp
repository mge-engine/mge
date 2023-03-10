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
        , m_command_buffer(VK_NULL_HANDLE)
    {
        allocate_command_buffer(context);
    }

    command_list::~command_list() { cleanup(); }

    void command_list::clear(const mge::rgba_color& color) {}

    void command_list::draw(const mge::draw_command& command) {}

    void command_list::finish() {}

    void command_list::execute() {}

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
        auto& ctx = dynamic_cast<render_context&>(context());
        if (m_command_buffer) {
            ctx.vkFreeCommandBuffers(ctx.device(),
                                     ctx.command_pool(),
                                     1,
                                     &m_command_buffer);
            m_command_buffer = VK_NULL_HANDLE;
        }
    }

} // namespace mge::vulkan