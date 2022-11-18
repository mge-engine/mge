
#include "vertex_buffer.hpp"

namespace mge::dx11 {
    vertex_buffer::vertex_buffer(render_context&      context,
                                 const vertex_layout& layout,
                                 size_t               data_size,
                                 void*                initial_data)
        : mge::vertex_buffer(context, layout, data_size, initial_data)
    {}

    vertex_buffer::~vertex_buffer() {}

    void* vertex_buffer::on_map() { return nullptr; }

    void vertex_buffer::on_unmap() {}

} // namespace mge::dx11