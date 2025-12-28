// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/extent.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/pass.hpp"
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
     * A render context also manages the submission of rendering commands
     * of the rendered frames by using the @c frame() method.
     */
    class MGEGRAPHICS_EXPORT render_context
    {
    protected:
        render_context(const mge::extent& ext);

    public:
        virtual ~render_context();

        /**
         * @brief Create a index buffer.
         *
         * @param dt            index buffer data type
         * @param data_size     size in bytes
         * @param data          initial data
         * @return created index buffer
         */
        virtual index_buffer* create_index_buffer(data_type dt,
                                                  size_t    data_size,
                                                  void*     data = nullptr) = 0;
        /**
         * @brief Destroy an index buffer.
         * @param ib index buffer to destroy
         */
        virtual void destroy_index_buffer(index_buffer* ib) = 0;

        /**
         * @brief Create a vertex buffer object.
         *
         * @param layout    vertex buffer layout
         * @param data_size size in bytes
         * @param data      initial data
         * @return created vertex buffer
         */
        virtual vertex_buffer* create_vertex_buffer(const vertex_layout& layout,
                                                    size_t data_size,
                                                    void*  data = nullptr) = 0;
        /**
         * @brief Destroy a vertex buffer.
         * @param vb vertex buffer to destroy
         */
        virtual void destroy_vertex_buffer(vertex_buffer* vb) = 0;

        /**
         * @brief Create a shader object.
         *
         * @param t             shader type
         * @return created shader
         */
        virtual shader* create_shader(shader_type t) = 0;

        
        shader_handle xcreate_shader(shader_type t);

        /**
         * @brief Destroy a shader.
         * @param s shader to destroy
         */
        virtual void destroy_shader(shader* s) = 0;

        /**
         * @brief Create a program object.
         *
         * @return created program
         */
        virtual program* create_program() = 0;
        /**
         * @brief Destroy a program.
         * @param p program to destroy
         */
        virtual void destroy_program(program* p) = 0;

        /**
         * @brief Create a command list object.
         *
         * @return command list
         */
        virtual command_list* create_command_list() = 0;
        /**
         * @brief Destroy a command list.
         * @param cl command list to destroy
         */
        virtual void destroy_command_list(command_list* cl) = 0;

        /**
         * @brief Create a command list object for the current frame.
         * @return command list
         */
        virtual frame_command_list* create_current_frame_command_list();
        /**
         * @brief Destroy a frame command list.
         * @param fcl frame command list to destroy
         */
        virtual void destroy_frame_command_list(frame_command_list* fcl) = 0;

        /**
         * @brief Create a texture object.
         *
         * @param type texture type
         * @return created texture
         */
        virtual texture_ref create_texture(texture_type type) = 0;

        /**
         * @brief Get the extent of the render context.
         *
         * @return extent
         */
        const mge::extent& extent() const noexcept
        {
            return m_extent;
        }

        ///////////////////////////////////////////////////////////////////
        mge::pass pass(uint32_t index);

        void      frame();
        image_ref screenshot();

    protected:
        mge::extent    m_extent;
        swap_chain_ref m_swap_chain;        //!< swap chain of this context
        uint32_t       m_index{0xFFFFFFFF}; //!< index in registry
    };

} // namespace mge