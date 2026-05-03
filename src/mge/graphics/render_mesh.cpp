// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/render_mesh.hpp"
#include "mge/graphics/command_buffer.hpp"
#include "mge/graphics/index_buffer.hpp"
#include "mge/graphics/mesh.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/vertex_buffer.hpp"

namespace mge {

    render_mesh::render_mesh(const vertex_buffer_handle& vertices,
                             const index_buffer_handle&  indices)
        : m_vertices(vertices)
        , m_indices(indices)
    {}

    render_mesh::render_mesh(render_context& context, const mesh& source)
        : m_vertices(context.create_vertex_buffer(
              source.layout(), source.vertex_data_size(), source.vertices()))
        , m_indices(context.create_index_buffer(source.index_element_type(),
                                                source.index_data_size(),
                                                source.indices()))
    {}

    const vertex_layout& render_mesh::layout() const
    {
        return m_vertices->layout();
    }

    data_type render_mesh::index_element_type() const
    {
        return m_indices->element_type();
    }

    size_t render_mesh::vertex_count() const
    {
        return m_vertices->element_count();
    }

    size_t render_mesh::index_count() const
    {
        return m_indices->element_count();
    }

    render_mesh::operator bool() const noexcept
    {
        return m_vertices && m_indices;
    }

    void render_mesh::draw(command_buffer&       command_buffer,
                           const program_handle& program,
                           uint32_t              index_count,
                           uint32_t              index_offset) const
    {
        command_buffer.draw(program,
                            m_vertices,
                            m_indices,
                            index_count,
                            index_offset);
    }

} // namespace mge
