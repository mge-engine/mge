// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/shader_type.hpp"
#include "mge/graphics/texture_type.hpp"
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
         * @brief Create a command list object for the current frame.
         * @return command list
         */
        virtual frame_command_list_ref create_current_frame_command_list();

        /**
         * @brief Create a texture object.
         *
         * @param type texture type
         * @return created texture
         */
        virtual texture_ref create_texture(texture_type type) = 0;

        /**
         * @brief Create a canvas.
         *
         * @return created canvas
         */
        virtual canvas_ref create_canvas() = 0;

        /**
         * @brief Get swap chain of context.
         *
         * @return swap chain
         */
        const swap_chain_ref& swap_chain() const;

    protected:
        render_context();

        swap_chain_ref m_swap_chain; //!< swap chain of this context
    };
} // namespace mge