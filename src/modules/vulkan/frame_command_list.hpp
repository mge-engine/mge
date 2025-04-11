// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "error.hpp"
#include "mge/graphics/frame_command_list.hpp"
#include "vulkan.hpp"

namespace mge::vulkan {
    class render_context;

    class frame_command_list : public mge::frame_command_list
    {
    public:
        frame_command_list(render_context& context,
                           uint64_t        frame,
                           uint32_t        backbuffer_index);
        virtual ~frame_command_list();

        void viewport(const mge::viewport& vp) override;
        void scissor(const mge::rectangle& r) override;
        void default_scissor() override;

        void clear(const mge::rgba_color& c) override;
        void clear_depth(float depth) override;
        void clear_stencil(int32_t stencil) override;
        void draw(const mge::draw_command& command) override;
        void execute() override;
        void finish() override;

    private:
        void begin();

        render_context& m_vulkan_context;
        uint64_t        m_frame{0};
        VkCommandBuffer m_command_buffer{VK_NULL_HANDLE};
    };

} // namespace mge::vulkan