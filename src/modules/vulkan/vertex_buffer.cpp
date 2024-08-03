#include "vertex_buffer.hpp"
#include "render_context.hpp"

namespace mge::vulkan {

    vertex_buffer::vertex_buffer(render_context&           context,
                                 const mge::vertex_layout& layout,
                                 size_t                    data_size,
                                 void*                     initial_data)
        : mge::vertex_buffer(context, layout, data_size, initial_data)
        , m_vulkan_context(context)
    {}

    vertex_buffer::~vertex_buffer() {}

    void* vertex_buffer::on_map() { return nullptr; }

    void vertex_buffer::on_unmap() {}

} // namespace mge::vulkan