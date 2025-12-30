// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "index_buffer.hpp"
#include "error.hpp"
#include "render_context.hpp"

namespace mge::vulkan {

    index_buffer::index_buffer(render_context& context,
                               mge::data_type  type,
                               size_t          data_size)
        : mge::index_buffer(context, type, data_size)
        , m_vulkan_context(context)
    {
        // can directly create buffer from any thread
        create_buffer();
    }

    void index_buffer::create_buffer()
    {
        VkBufferCreateInfo buffer_info{};
        buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buffer_info.size = size();
        buffer_info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo alloc_info{};
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

    index_buffer::~index_buffer()
    {
        if (m_buffer && m_allocation) {
            vmaDestroyBuffer(m_vulkan_context.allocator(),
                             m_buffer,
                             m_allocation);
            m_buffer = VK_NULL_HANDLE;
            m_allocation = VK_NULL_HANDLE;
        }
    }

    void index_buffer::on_set_data(void* data, size_t data_size)
    {
        if (!data || data_size == 0) {
            return;
        }

        if (data_size > size()) {
            MGE_THROW(vulkan::error) << "Data size " << data_size
                                     << " exceeds buffer size " << size();
        }

        void* mapped_data = nullptr;
        CHECK_VK_CALL(vmaMapMemory(m_vulkan_context.allocator(),
                                   m_allocation,
                                   &mapped_data));

        memcpy(mapped_data, data, data_size);

        CHECK_VK_CALL(vmaFlushAllocation(m_vulkan_context.allocator(),
                                         m_allocation,
                                         0,
                                         data_size));

        vmaUnmapMemory(m_vulkan_context.allocator(), m_allocation);
    }

    VkIndexType index_buffer::vk_index_type() const
    {
        switch (element_type()) {
        case mge::data_type::INT16:
        case mge::data_type::UINT16:
            return VK_INDEX_TYPE_UINT16;
        case mge::data_type::INT32:
        case mge::data_type::UINT32:
            return VK_INDEX_TYPE_UINT32;
        default:
            MGE_THROW(vulkan::error)
                << "Unsupported index buffer element type " << element_type();
        }
    }

} // namespace mge::vulkan