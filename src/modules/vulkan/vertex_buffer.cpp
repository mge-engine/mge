// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "vertex_buffer.hpp"
#include "error.hpp"
#include "render_context.hpp"

namespace mge::vulkan {

    vertex_buffer::vertex_buffer(render_context&           context,
                                 const mge::vertex_layout& layout,
                                 size_t                    data_size)
        : mge::vertex_buffer(context, layout, data_size)
        , m_vulkan_context(context)
    {
        create_buffer();
        m_binding_description.binding = 0;
        m_binding_description.stride = static_cast<uint32_t>(layout.stride());
        m_binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        set_ready(true);
    }
    void vertex_buffer::create_buffer()
    {
        VkBufferCreateInfo buffer_info{};
        buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buffer_info.size = size();
        buffer_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo alloc_info = {};
        alloc_info.usage = VMA_MEMORY_USAGE_AUTO;
        alloc_info.flags =
            VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

        CHECK_VK_CALL(vmaCreateBuffer(m_vulkan_context.allocator(),
                                      &buffer_info,
                                      &alloc_info,
                                      &m_buffer,
                                      &m_allocation,
                                      nullptr));
    }

    vertex_buffer::~vertex_buffer()
    {
        if (m_buffer && m_allocation) {
            vmaDestroyBuffer(m_vulkan_context.allocator(),
                             m_buffer,
                             m_allocation);
            m_buffer = VK_NULL_HANDLE;
            m_allocation = VK_NULL_HANDLE;
        }
    }

    void vertex_buffer::on_set_data(void* data, size_t data_size)
    {
        void* mapped_data = nullptr;
        CHECK_VK_CALL(vmaMapMemory(m_vulkan_context.allocator(),
                                   m_allocation,
                                   &mapped_data));
        std::memcpy(mapped_data, data, data_size);
        CHECK_VK_CALL(vmaFlushAllocation(m_vulkan_context.allocator(),
                                         m_allocation,
                                         0,
                                         VK_WHOLE_SIZE));
        set_ready(true);
        vmaUnmapMemory(m_vulkan_context.allocator(), m_allocation);
    }

    const std::vector<VkVertexInputAttributeDescription>&
    vertex_buffer::attribute_descriptions() const
    {
        return m_vulkan_context.vertex_input_attribute_descriptions(layout());
    }

} // namespace mge::vulkan