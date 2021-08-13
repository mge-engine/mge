// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/vertex_buffer.hpp"

namespace mge {
    vertex_buffer::vertex_buffer(render_context&      context,
                                 const vertex_layout& layout,
                                 size_t               data_size,
                                 void*                initial_data)
        : hardware_buffer(context, buffer_type::VERTEX, data_size, initial_data)
        , m_layout(layout)
    {}

    vertex_buffer::~vertex_buffer() {}

    const vertex_layout& vertex_buffer::layout() const { return m_layout; }

    size_t vertex_buffer::element_count() const { return size() / m_layout.binary_size(); }

} // namespace mge