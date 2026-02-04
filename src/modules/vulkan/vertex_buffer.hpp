// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/vertex_buffer.hpp"
#include "vulkan.hpp"

namespace mge::vulkan {

    class render_context;

    inline VkFormat vk_format(const mge::vertex_format& fmt) noexcept
    {
        if (fmt.type() == mge::data_type::FLOAT) {
            switch (fmt.size()) {
            case 1:
                return VK_FORMAT_R32_SFLOAT;
            case 2:
                return VK_FORMAT_R32G32_SFLOAT;
            case 3:
                return VK_FORMAT_R32G32B32_SFLOAT;
            case 4:
                return VK_FORMAT_R32G32B32A32_SFLOAT;
            }
        }
        return VK_FORMAT_UNDEFINED;
    }

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

        const auto& attribute_descriptions() const;

        void on_set_data(void* data, size_t data_size) override;

    private:
        void create_buffer();

        render_context&                 m_vulkan_context;
        VkBuffer                        m_buffer{VK_NULL_HANDLE};
        VmaAllocation                   m_allocation{VK_NULL_HANDLE};
        VkVertexInputBindingDescription m_binding_description;
    };

} // namespace mge::vulkan