#include "index_buffer.hpp"
#include "error.hpp"
#include "render_context.hpp"

namespace mge::vulkan {

    index_buffer::index_buffer(render_context& context,
                               mge::data_type  type,
                               size_t          data_size,
                               void*           data)
        : mge::index_buffer(context, type, data_size, data)
        , m_vulkan_context(context)
    {
        create_buffer();
        // TODO: maybe use staging buffer
        if (data) {
            void* mapped_data = map();
            memcpy(mapped_data, data, size());
            unmap();
        }
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

    void* index_buffer::on_map()
    {
        void* data = nullptr;
        CHECK_VK_CALL(
            vmaMapMemory(m_vulkan_context.allocator(), m_allocation, &data));
        return data;
    }

    void index_buffer::on_unmap()
    {
        CHECK_VK_CALL(vmaFlushAllocation(m_vulkan_context.allocator(),
                                         m_allocation,
                                         0,
                                         VK_WHOLE_SIZE));
        vmaUnmapMemory(m_vulkan_context.allocator(), m_allocation);
    }

} // namespace mge::vulkan