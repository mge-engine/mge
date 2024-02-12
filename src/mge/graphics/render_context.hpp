// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/command_sequence.hpp"
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/draw_command.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/shader_type.hpp"
#include "mge/graphics/vertex_layout.hpp"
#include <memory>

namespace mge {

    /**
     * A render context is a context in which objects that are elements
     * of the render pipeline are created - such as shader programs,
     * textures and buffers.
     *
     * A render context has a swap chain to facilitate presentation
     * of the rendered frames.
     */
    class MGEGRAPHICS_EXPORT render_context
        : public std::enable_shared_from_this<render_context>
    {
    public:
        virtual ~render_context() = default;

        /**
         * @brief Create a index buffer.
         *
         * @param dt            index buffer data type
         * @param data_size     size in bytes
         * @param data          initial data
         * @return created index buffer
         */
        virtual index_buffer_ref create_index_buffer(data_type dt,
                                                     size_t    data_size,
                                                     void* data = nullptr) = 0;

        /**
         * @brief Create a vertex buffer object.
         *
         * @param layout    vertex buffer layout
         * @param data_size size in bytes
         * @param data      initial data
         * @return created vertex buffer
         */
        virtual vertex_buffer_ref
        create_vertex_buffer(const vertex_layout& layout,
                             size_t               data_size,
                             void*                data = nullptr) = 0;

        /**
         * @brief Create a shader object.
         *
         * @param t             shader type
         * @return created shader
         */
        virtual shader_ref create_shader(shader_type t) = 0;

        /**
         * @brief Create a program object.
         *
         * @return created program
         */
        virtual program_ref create_program() = 0;

        /**
         * @brief Create a command list object.
         *
         * @return command list
         */
        virtual command_list_ref create_command_list() = 0;

        /**
         * @brief Create a command sequence.
         *
         * @return empty command sequence
         */
        virtual command_sequence_ref create_command_sequence() = 0;

        /**
         * @brief Execute a command sequence.
         *
         * @param sequence command sequence
         */
        virtual void execute(const command_sequence& sequence) = 0;

        /**
         * @brief Get swap chain of context.
         *
         * @return swap chain
         */
        const swap_chain_ref& swap_chain() const;

        /**
         * @brief Clears the color buffer with the requested color.
         *
         * @param color clear color
         */
        virtual void clear(const rgba_color& c) = 0;

        /**
         * @brief Create a draw command.
         *
         * @param program program to draw
         * @param vertices  vectices
         * @param indices  indices
         * @param t topology
         * @return draw command
         */
        virtual draw_command_ref
        create_draw_command(const program_ref&       program,
                            const vertex_buffer_ref& vertices,
                            const index_buffer_ref&  indices,
                            topology                 t) = 0;

        /**
         * @brief Draws a draw command.
         *
         * @param command draw command
         */
        virtual void draw(const draw_command& command) = 0;

    protected:
        render_context();

        swap_chain_ref m_swap_chain; //!< swap chain of this context
    };
} // namespace mge