// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/callable.hpp"
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/extent.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/pass.hpp"
#include "mge/graphics/program_handle.hpp"
#include "mge/graphics/shader_handle.hpp"
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

    protected:
        /**
         * @brief Create a shader object.
         *
         * @param t             shader type
         * @return created shader
         */
        virtual shader* on_create_shader(shader_type t) = 0;

        /**
         * @brief Destroy a shader.
         * @param s shader to destroy
         */
        virtual void on_destroy_shader(shader* s) = 0;

    public:
        shader_handle create_shader(shader_type t);
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

        /**
         * @brief Get the index of this render context.
         * @return context index
         */
        uint16_t index() const noexcept
        {
            return m_index;
        }

        /**
         * @brief Get render context by index.
         * @param index context index
         * @return render context or nullptr if not found
         */
        static render_context* get(uint16_t index);

        /**
         * @brief Get an object by its handle.
         *
         * @tparam T object type
         * @param index context index
         * @param flags object flags
         * @param object_index object index
         * @return object pointer or nullptr if not found
         */
        template <typename T>
        T* object(uint16_t index, uint16_t flags, uint32_t object_index)
        {
            if constexpr (std::is_same_v<T, shader>) {
                if (object_index < m_shaders.size()) {
                    return m_shaders[object_index];
                }
            } else if constexpr (std::is_same_v<T, program>) {
                if (object_index < m_programs.size()) {
                    return m_programs[object_index];
                }
            } else if constexpr ((std::is_same_v<T, hardware_buffer>) ||
                                 (std::is_base_of_v<hardware_buffer, T>)) {
                if (object_index < m_buffers.size()) {
                    return m_buffers[object_index];
                }
            }
            return nullptr;
        }

        template <typename T>
        void
        destroy_object(uint16_t index, uint16_t flags, uint32_t object_index)
        {
            if constexpr (std::is_same_v<T, shader>) {
                if (object_index < m_shaders.size()) {
                    on_destroy_shader(m_shaders[object_index]);
                    m_shaders[object_index] = nullptr;
                }
            }
        }

        template <mge::callable ActionType>
        void prepare_frame(ActionType&& action)
        {
            m_prepare_frame_actions.emplace_back(
                std::forward<ActionType>(action));
        }

    protected:
        mge::extent    m_extent;
        swap_chain_ref m_swap_chain;    //!< swap chain of this context
        uint16_t       m_index{0xFFFF}; //!< index in registry

        std::vector<shader*>          m_shaders;
        std::vector<program*>         m_programs;
        std::vector<hardware_buffer*> m_buffers;

        using prepare_frame_action = std::function<void()>;
        std::vector<prepare_frame_action> m_prepare_frame_actions;
    };

} // namespace mge