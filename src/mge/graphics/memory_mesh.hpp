// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/buffer.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/mesh.hpp"

namespace mge {
    /**
     * @brief Memory mesh.
     *
     * A memory mesh is a mesh that is stored in memory.
     */
    class MGEGRAPHICS_EXPORT memory_mesh : public mesh
    {
    public:
        memory_mesh(const vertex_layout& layout,
                    data_type            index_element_type,
                    size_t               vertex_size,
                    size_t               index_size);

        memory_mesh(const memory_mesh&) = delete;
        memory_mesh(memory_mesh&&) = delete;
        memory_mesh& operator=(const memory_mesh&) = delete;
        memory_mesh& operator=(memory_mesh&&) = delete;

        virtual ~memory_mesh();

        size_t vertex_data_size() const override
        {
            return m_vertices->size();
        }

        size_t index_data_size() const override
        {
            return m_indices->size();
        }

        mge::buffer_ref vertices() const override
        {
            return m_vertices;
        }

        mge::buffer_ref indices() const override
        {
            return m_indices;
        }

        void* vertex_data() const override
        {
            return static_cast<void*>(const_cast<uint8_t*>(m_vertices->data()));
        }

        void* index_data() const override
        {
            return static_cast<void*>(const_cast<uint8_t*>(m_indices->data()));
        }

        std::span<uint8_t> vertex_data_span() const override
        {
            return std::span<uint8_t>(const_cast<uint8_t*>(m_vertices->data()),
                                      m_vertices->size());
        }

        std::span<uint8_t> index_data_span() const override
        {
            return std::span<uint8_t>(const_cast<uint8_t*>(m_indices->data()),
                                      m_indices->size());
        }

    private:
        mge::buffer_ref m_vertices;
        mge::buffer_ref m_indices;
    };
} // namespace mge