// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/mesh.hpp"
namespace mge {

    class MGE_GRAPHICS_EXPORT memory_mesh : public mesh
    {
    public:
        memory_mesh(const vertex_layout& layout, size_t num_vertices, size_t num_indices);
        virtual ~memory_mesh();

        void *vertex_data() const { return m_vertex_data; }
        void *index_data() const { return m_index_data; }

        const mge::vertex_layout& layout() const noexcept override { return m_vertex_layout; }
        size_t num_vertices() const noexcept override { return m_num_vertices; }
        size_t num_indices() const noexcept override { return m_num_indices; }
        vertex_buffer_ref vertices(render_context& context) const override;
        index_buffer_ref indices(render_context& context) const override;
    private:
        vertex_layout m_vertex_layout;
        size_t        m_num_vertices;
        size_t        m_num_indices;
        void         *m_vertex_data;
        void         *m_index_data;
    };
}