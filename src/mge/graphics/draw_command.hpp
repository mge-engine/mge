// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/topology.hpp"
namespace mge {

    /**
     * @brief A draw command executes a program with a given set of buffers.
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
         * @param program program to be executed
         * @param vertices vertices
         * @param indices  indices
         * @param t        vertices topology
         */
        draw_command(const program_ref&       program,
                     const vertex_buffer_ref& vertices,
                     const index_buffer_ref&  indices,
                     topology                 t);

        /**
         * @brief Copy constructor.
         * @param cmd command to copy
         */
        draw_command(const draw_command& cmd) = default;

        /**
         * @brief Move constructor.
         * @param cmd command to move
         */
        draw_command(draw_command&&) = default;

        ~draw_command();

        /**
         * @brief Access stored program.
         * @return program
         */
        const program_ref& program() const { return m_program; }
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
        const program_ref       m_program;
        const vertex_buffer_ref m_vertices;
        const index_buffer_ref  m_indices;
        const mge::topology     m_topology;
    };

} // namespace mge