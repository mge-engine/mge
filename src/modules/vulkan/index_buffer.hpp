// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/index_buffer.hpp"
#include "vulkan.hpp"

namespace mge::vulkan {

    class render_context;

    class index_buffer : public mge::index_buffer
    {
    public:
        index_buffer(render_context& context,
                     mge::data_type  typem,
                     size_t          data_size);

        ~index_buffer();

        VkBuffer vk_buffer() const
        {
            return m_buffer;
        }

        VkIndexType vk_index_type() const;

        void on_set_data(void* data, size_t data_size) override;

    private:
        void create_buffer();

        render_context& m_vulkan_context;
        VkBuffer        m_buffer{VK_NULL_HANDLE};
        VmaAllocation   m_allocation{VK_NULL_HANDLE};
    };

} // namespace mge::vulkan
