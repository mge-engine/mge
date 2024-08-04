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

    protected:
        virtual void* on_map() override;
        virtual void  on_unmap() override;

    private:
        render_context& m_vulkan_context;
    };

} // namespace mge::vulkan