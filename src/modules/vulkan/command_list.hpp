// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/command_list.hpp"
#include "vulkan.hpp"

#include <variant>
#include <vector>

namespace mge::vulkan {
    class render_context;

    class command_list : public mge::command_list
    {
    public:
        command_list(render_context& context);
        ~command_list();

        void clear(const mge::rgba_color& color) override;
        void draw(const mge::draw_command& command) override;
        void finish() override;
        void execute() override;

        void                   record_on_frame(VkFramebuffer frame_buffer);
        inline VkCommandBuffer command_buffer() const noexcept
        {
            return m_command_buffer;
        }

    private:
        void allocate_command_buffer(render_context& context);
        void cleanup();

        render_context& m_vulkan_context;
        VkCommandBuffer m_command_buffer;
        rgba_color      m_clear_color;
    };
} // namespace mge::vulkan