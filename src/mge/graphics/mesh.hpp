// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/buffer.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/vertex_layout.hpp"

#include <cstddef>
#include <span>

namespace mge {

    /**
     * @brief A mesh is a collection of vertices and indices stored in memory.
     *
     * A mesh is a concrete buffer of vertex and index data. Use @c render_mesh
     * to upload mesh data to the GPU for rendering.
     */
    class MGEGRAPHICS_EXPORT mesh
    {
    public:
        mesh(const vertex_layout& layout,
             data_type            index_element_type,
             size_t               vertex_size,
             size_t               index_size);

        mesh(const mesh&) = delete;
        mesh(mesh&&) = delete;
        mesh& operator=(const mesh&) = delete;
        mesh& operator=(mesh&&) = delete;

        ~mesh() = default;

        const vertex_layout& layout() const { return m_vertex_layout; }

        data_type index_element_type() const noexcept
        {
            return m_index_element_type;
        }

        size_t vertex_data_size() const { return m_vertices->size(); }

        size_t index_data_size() const { return m_indices->size(); }

        void* vertex_data() const
        {
            return static_cast<void*>(
                const_cast<std::byte*>(m_vertices->data()));
        }

        void* index_data() const
        {
            return static_cast<void*>(
                const_cast<std::byte*>(m_indices->data()));
        }

        std::span<std::byte> vertex_data_span() const
        {
            return std::span<std::byte>(
                const_cast<std::byte*>(m_vertices->data()),
                m_vertices->size());
        }

        std::span<std::byte> index_data_span() const
        {
            return std::span<std::byte>(
                const_cast<std::byte*>(m_indices->data()),
                m_indices->size());
        }

        buffer_ref vertices() const { return m_vertices; }

        buffer_ref indices() const { return m_indices; }

    private:
        vertex_layout m_vertex_layout;
        data_type     m_index_element_type;
        buffer_ref    m_vertices;
        buffer_ref    m_indices;
    };

} // namespace mge
