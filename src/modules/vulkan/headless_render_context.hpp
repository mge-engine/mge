// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "render_context_base.hpp"

namespace mge::vulkan {
    class render_system;

    class headless_render_context : public render_context_base
    {
    public:
        headless_render_context(mge::vulkan::render_system& rs,
                                const mge::extent&          ext);
        ~headless_render_context() override;

        void           on_frame_present() override;
        void           render(const mge::pass& p) override;
        mge::image_ref screenshot() override;

    protected:
        std::vector<const char*> get_device_extensions() const override;

    private:
        void create_command_buffer();
        void create_render_fence();
        void teardown();

        enum class frame_state
        {
            BEFORE_DRAW,
            DRAW
        };

        VkCommandBuffer    m_command_buffer{VK_NULL_HANDLE};
        VkFence            m_fence{VK_NULL_HANDLE};
        frame_state        m_frame_state{frame_state::BEFORE_DRAW};
        mge::frame_buffer* m_last_frame_buffer{nullptr};
    };

} // namespace mge::vulkan
