// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/memory_mesh.hpp"

namespace mge {
    memory_mesh::memory_mesh(const vertex_layout& layout,
                             data_type            index_element_type,
                             size_t               vertex_size,
                             size_t               index_size)
        : mesh(layout, index_element_type)
    {
        m_vertices = mge::make_buffer(vertex_size);
        m_indices = mge::make_buffer(index_size);
    }

    memory_mesh::~memory_mesh() {}
} // namespace mge