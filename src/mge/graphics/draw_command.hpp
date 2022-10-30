// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/topology.hpp"
namespace mge {

    /**
     * @brief A draw command executes a pipeline with a given set of buffers.
     */
    class MGEGRAPHICS_EXPORT draw_command
    {
    public:
        /**
         * @brief Construct empty draw command.
         */
        draw_command();

        /**
         * @brief Construct a new draw command.
         *
         * @param pipeline pipeline to be executed
         * @param vertices vertices
         * @param indices  indices
         * @param t        vertices topology
         */
        draw_command(const pipeline_ref&      pipeline,
                     const vertex_buffer_ref& vertices,
                     const index_buffer_ref&  indices,
                     topology                 t);

        ~draw_command();

        /**
         * @brief Access stored pipeline.
         * @return pipeline
         */
        const pipeline_ref& pipeline() const { return m_pipeline; }
        /**
         * @brief Access stored vertex buffer.
         * @return vertex buffer
         */
        const vertex_buffer_ref& vertices() const { return m_vertices; }
        /**
         * @brief Access stored index buffer.
         *
         * @return const index buffer
         */
        const index_buffer_ref& indices() const { return m_indices; }

        /**
         * @brief Access stored topology.
         *
         * @return mge::topology
         */
        mge::topology topology() const { return m_topology; }

    private:
        const pipeline_ref      m_pipeline;
        const vertex_buffer_ref m_vertices;
        const index_buffer_ref  m_indices;
        const mge::topology     m_topology;
    };

} // namespace mge