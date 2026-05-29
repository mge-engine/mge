// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/mesh.hpp"

namespace mge {

    mesh::mesh(const vertex_layout& layout,
               data_type            index_element_type,
               size_t               vertex_size,
               size_t               index_size)
        : m_vertex_layout(layout)
        , m_index_element_type(index_element_type)
        , m_vertices(mge::make_buffer(vertex_size))
        , m_indices(mge::make_buffer(index_size))
    {}

} // namespace mge
