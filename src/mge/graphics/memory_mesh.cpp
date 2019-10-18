// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/memory_mesh.hpp"
#include "mge/core/memory.hpp"
#include "mge/graphics/render_context.hpp"
namespace mge {
    memory_mesh::memory_mesh(const vertex_layout& layout, size_t num_vertices, size_t num_indices)
        :m_vertex_layout(layout)
        ,m_num_vertices(num_vertices)
        ,m_num_indices(num_indices)
        ,m_vertex_data(nullptr)
        ,m_index_data(nullptr)
    {
        m_vertex_data = mge::malloc(m_vertex_layout.binary_size() * m_num_vertices);
        m_index_data = mge::malloc(sizeof(uint32_t) * m_num_indices);
    }

    memory_mesh::~memory_mesh()
    {
        if (m_vertex_data) {
            mge::free(m_vertex_data);
        }
        if (m_index_data) {
            mge::free(m_index_data);
        }
    }

    mge::vertex_buffer_ref
    memory_mesh::vertices(render_context& c) const
    {
        return c.create_vertex_buffer(layout(), 
                                      mge::usage::DEFAULT,
                                      m_num_vertices,
                                      m_vertex_data);
    }

    mge::index_buffer_ref
    memory_mesh::indices(render_context& c) const
    {
        return c.create_index_buffer(mge::data_type::UINT32,
                                     mge::usage::DEFAULT,
                                     m_num_indices,
                                     m_index_data);
    }
}