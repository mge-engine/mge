#include "mge/graphics/vertex_buffer.hpp"

namespace mge {
    vertex_buffer::vertex_buffer(render_context& context,
                                 const vertex_layout& layout,
                                 usage usage,
                                 size_t element_count,
                                 void *initial_data)
        :hardware_buffer(context,
                         buffer_type::VERTEX_BUFFER,
                         usage,
                         initial_data,
                         layout.binary_size() * element_count)
        ,m_layout(layout)
    {}

    vertex_buffer::~vertex_buffer()
    {}

    size_t 
    vertex_buffer::element_count() const
    {
        return size() / m_layout.binary_size();
    }
}
