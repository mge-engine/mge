// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/pipeline.hpp"
#include "mge/graphics/vertex_buffer.hpp"
#include "mge/graphics/index_buffer.hpp"


namespace mge {

    /**
     * Command to execute a pipeline using specified settings.
     */
    class MGE_GRAPHICS_EXPORT draw_command
    {
    public:
        /**
         * Create empty command.
         */
        draw_command();

        /**
         * Construct command with pipline and buffer reference.
         * @param p pipeline
         * @param v vertices
         * @param i indices
         */
        draw_command(const pipeline_ref& p,
                const vertex_buffer_ref& v,
                const index_buffer_ref& i);

        /**
         * Copy constructor.
         * @param c copied command
         */
        draw_command(const draw_command& c);
        /**
         * Move constructor.
         * @param c moved command
         */
        draw_command(draw_command&& c);

        /**
         * Destructor.
         */
        ~draw_command() = default;

        draw_command& operator =(const draw_command& c);
        draw_command& operator =(draw_command&& c);

        void set_pipeline(const pipeline_ref& pipeline);
        void set_vertices(const vertex_buffer_ref& buffer);
        void set_indices(const index_buffer_ref& buffer);

        const pipeline_ref& pipeline() const { return m_pipeline; }
        const vertex_buffer_ref& vertices() const { return m_vertices; }
        const index_buffer_ref& indices() const { return m_indices; }
    private:
        pipeline_ref m_pipeline;
        vertex_buffer_ref m_vertices;
        index_buffer_ref m_indices;
    };

}
