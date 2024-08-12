#include "vertex_buffer.hpp"
#include "error.hpp"
#include "render_context.hpp"

namespace mge::vulkan {

    vertex_buffer::vertex_buffer(render_context&           context,
                                 const mge::vertex_layout& layout,
                                 size_t                    data_size,
                                 void*                     initial_data)
        : mge::vertex_buffer(context, layout, data_size, initial_data)
        , m_vulkan_context(context)
    {
        create_buffer();
        m_binding_description.binding = 0;
        m_binding_description.stride = static_cast<uint32_t>(layout.stride());
        m_binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        fill_attribute_descriptions();
    }

    static inline VkFormat vk_format(const mge::vertex_format& fmt)
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

    void vertex_buffer::fill_attribute_descriptions()
    {
        m_attribute_descriptions.clear();
        uint32_t offset = 0;
        uint32_t location = 0;
        for (const auto& el : layout()) {
            VkVertexInputAttributeDescription desc;
            desc.binding = 0;
            desc.location = location++;
            desc.format = vk_format(el.format);
            desc.offset = offset;
            m_attribute_descriptions.push_back(desc);
            offset += static_cast<uint32_t>(el.format.binary_size());
        }
    }

    void vertex_buffer::create_buffer()
    {
        VkBufferCreateInfo buffer_info{};
        buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buffer_info.size = size();
        buffer_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        CHECK_VK_CALL(m_vulkan_context.vkCreateBuffer(m_vulkan_context.device(),
                                                      &buffer_info,
                                                      nullptr,
                                                      &m_buffer));

        // VkMemoryRequirements memRequirements;
        // vkGetBufferMemoryRequirements(device, vertexBuffer,
        // &memRequirements);
    }

    vertex_buffer::~vertex_buffer()
    {
        if (m_buffer && m_vulkan_context.vkDestroyBuffer) {
            m_vulkan_context.vkDestroyBuffer(m_vulkan_context.device(),
                                             m_buffer,
                                             nullptr);
            m_buffer = VK_NULL_HANDLE;
        }
    }

    void* vertex_buffer::on_map() { return nullptr; }

    void vertex_buffer::on_unmap() {}

} // namespace mge::vulkan