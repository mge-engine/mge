#pragma once
#include "error.hpp"
#include "mge/graphics/frame_command_list.hpp"
#include "vulkan.hpp"

namespace mge::vulkan {
    class render_context;

    class frame_command_list : public mge::frame_command_list
    {
    public:
        frame_command_list(render_context& context, uint32_t backbuffer_index);
        virtual ~frame_command_list();

        void clear(const mge::rgba_color& c) override;
        void draw(const mge::draw_command& command) override;
        void execute() override;
        void finish() override;

    private:
        render_context& m_vulkan_context;
        VkCommandBuffer m_command_buffer{VK_NULL_HANDLE};
    };

} // namespace mge::vulkan