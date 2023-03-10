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

        void record_on_frame(uint32_t image);

    private:
        void allocate_command_buffer(render_context& context);
        void cleanup();

        struct clear_command
        {
            rgba_color clear_color;
        };

        using command = std::variant<std::monostate, clear_command>;
        using command_vector = std::vector<command>;

        render_context& m_vulkan_context;
        VkCommandBuffer m_command_buffer;
        command_vector  m_commands;
    };
} // namespace mge::vulkan