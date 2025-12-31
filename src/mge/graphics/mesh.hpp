// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/buffer.hpp"
#include "mge/core/format.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/vertex_layout.hpp"
#include <span>

namespace mge {

    /**
     * @brief A mesh is a collection of vertices and indices.
     *
     * Note that a mesh is not a vertex or index buffer, but the representation
     * vertices and indices in memory (or accessible). A mesh can be used to
     * create vertex and index buffers, but is not necessarily a buffer itself.
     */
    class MGEGRAPHICS_EXPORT mesh
    {
    protected:
        /**
         * @brief Construct a new mesh object.
         *
         * @param layout vertex layout
         * @param index_element_type type of index buffer elements, default
         * UINT32
         */
        mesh(const vertex_layout& layout,
             data_type            index_element_type = data_type::UINT32);

    public:
        virtual ~mesh() = default;

        /**
         * @brief Get the vertex layout of the mesh.
         *
         * @return vertex layout of the mesh
         */
        const vertex_layout& layout() const
        {
            return m_vertex_layout;
        }

        /**
         * @brief Size of the vertex data in bytes.
         * @return size of the vertex data in bytes
         */
        virtual size_t vertex_data_size() const = 0;

        /**
         * @brief Size of the index data in bytes.
         * @return size of the index data in bytes
         */
        virtual size_t index_data_size() const = 0;

        /**
         * @brief Raw vertex data.
         * @return vertex data
         */
        virtual void* vertex_data() const = 0;

        /**
         * @brief Raw index data.
         * @return index data
         */
        virtual void* index_data() const = 0;

        /**
         * @brief Vertex data of the mesh.
         *
         * @return vertex data of the mesh
         */
        virtual std::span<uint8_t> vertex_data_span() const = 0;

        /**
         * @brief Index data of the mesh.
         *
         * @return index data of the mesh
         */
        virtual std::span<uint8_t> index_data_span() const = 0;

        /**
         * @brief Get the vertex buffer as buffer reference.
         * Depending on the implementation of the mesh, this may return a copy.
         *
         * @return vertex buffer reference
         */
        virtual buffer_ref vertices() const = 0;

        /**
         * @brief Get the index buffer reference.
         * Depending on the implementation of the mesh, this may return a copy.
         *
         * @return index buffer reference
         */
        virtual buffer_ref indices() const = 0;

    private:
        vertex_layout m_vertex_layout;
        data_type     m_index_element_type;
    };

} // namespace mge