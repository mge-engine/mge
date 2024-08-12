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
                      size_t                    data_size,
                      void*                     initial_data = nullptr);

        ~vertex_buffer();

        VkVertexInputBindingDescription          binding_description() const;
        const VkVertexInputAttributeDescription* attribute_description() const
        {
            return m_attribute_descriptions.data();
        }

    protected:
        virtual void* on_map() override;
        virtual void  on_unmap() override;

    private:
        void create_buffer();
        void fill_attribute_descriptions();

        render_context&                                m_vulkan_context;
        VkBuffer                                       m_buffer{VK_NULL_HANDLE};
        VkVertexInputBindingDescription                m_binding_description;
        std::vector<VkVertexInputAttributeDescription> m_attribute_descriptions;
    };

} // namespace mge::vulkan