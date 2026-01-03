// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/vertex_buffer.hpp"
#include "vulkan.hpp"

namespace mge::vulkan {

    class render_context;

    class vertex_buffer : public mge::vertex_buffer
    {
    public:
        vertex_buffer(render_context&           context,
                      const mge::vertex_layout& layout,
                      size_t                    data_size);

        ~vertex_buffer();

        VkBuffer vk_buffer() const
        {
            return m_buffer;
        }

        const VkVertexInputBindingDescription& binding_description() const
        {
            return m_binding_description;
        }

        const auto& attribute_descriptions() const
        {
            return m_attribute_descriptions;
        }

        void on_set_data(void* data, size_t data_size) override;

    private:
        void create_buffer();
        void fill_attribute_descriptions();

        render_context&                 m_vulkan_context;
        VkBuffer                        m_buffer{VK_NULL_HANDLE};
        VmaAllocation                   m_allocation{VK_NULL_HANDLE};
        VkVertexInputBindingDescription m_binding_description;
        std::vector<VkVertexInputAttributeDescription> m_attribute_descriptions;
    };

} // namespace mge::vulkan