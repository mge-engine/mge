// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/frame_buffer.hpp"
#include "mge/graphics/frame_buffer_info.hpp"
#include "vulkan.hpp"

namespace mge::vulkan {

    class render_context_base;

    class frame_buffer : public mge::frame_buffer
    {
    public:
        frame_buffer(render_context_base&          ctx,
                     const mge::frame_buffer_info& info);
        ~frame_buffer() override;

        VkRenderPass render_pass() const noexcept
        {
            return m_render_pass;
        }

        VkFramebuffer vk_framebuffer() const noexcept
        {
            return m_framebuffer;
        }

        VkExtent2D fbo_extent() const noexcept
        {
            return m_extent;
        }

    protected:
        void on_attach_color(const mge::texture_ref& tex, uint32_t slot) override;
        void on_attach_depth(const mge::texture_ref& tex) override;

    private:
        void create_render_pass(const mge::frame_buffer_info& info);
        void create_framebuffer();

        render_context_base& m_context;
        VkRenderPass    m_render_pass{VK_NULL_HANDLE};
        VkFramebuffer   m_framebuffer{VK_NULL_HANDLE};
        VkExtent2D      m_extent{};
    };

} // namespace mge::vulkan
