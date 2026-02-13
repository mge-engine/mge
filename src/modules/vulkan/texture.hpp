// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/texture.hpp"
#include "vulkan.hpp"

namespace mge::vulkan {
    class render_context;

    class texture : public mge::texture
    {
    public:
        texture(render_context& context, mge::texture_type type);
        ~texture() override;

        void set_data(const mge::image_format& format,
                      const mge::extent&       extent,
                      const void*              data,
                      size_t                   size) override;

        VkImageView image_view() const noexcept
        {
            return m_image_view;
        }

        VkSampler sampler() const noexcept
        {
            return m_sampler;
        }

    private:
        VkFormat texture_format(const mge::image_format& format) const;
        void     create_image(VkFormat format, uint32_t width, uint32_t height);
        void     create_image_view(VkFormat format);
        void     create_sampler();
        void     upload_data(const void* data,
                             size_t      size,
                             uint32_t    width,
                             uint32_t    height,
                             size_t      row_pitch);
        void     transition_image_layout(VkCommandBuffer command_buffer,
                                         VkImageLayout   old_layout,
                                         VkImageLayout   new_layout);

        VkImage       m_image{VK_NULL_HANDLE};
        VmaAllocation m_allocation{VK_NULL_HANDLE};
        VkImageView   m_image_view{VK_NULL_HANDLE};
        VkSampler     m_sampler{VK_NULL_HANDLE};
    };
} // namespace mge::vulkan
