// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/buffer.hpp"
#include "mge/core/callable.hpp"
#include "mge/graphics/command_buffer.hpp"
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/extent.hpp"
#include "mge/graphics/frame_buffer_handle.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/index_buffer_handle.hpp"
#include "mge/graphics/pass.hpp"
#include "mge/graphics/program_handle.hpp"
#include "mge/graphics/shader_handle.hpp"
#include "mge/graphics/shader_type.hpp"
#include "mge/graphics/texture_type.hpp"
#include "mge/graphics/vertex_buffer_handle.hpp"
#include "mge/graphics/vertex_layout.hpp"

#include <memory>
#include <memory_resource>

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
        render_context(mge::render_system& rs, const mge::extent& ext);

    public:
        virtual ~render_context();

    protected:
        /**
         * @brief Create a index buffer.
         *
         * @param dt            index buffer data type
         * @param data_size     size in bytes
         * @param data          initial data
         * @return created index buffer
         */
        virtual index_buffer* on_create_index_buffer(data_type dt,
                                                     size_t    data_size) = 0;
        /**
         * @brief Destroy an index buffer.
         * @param ib index buffer to destroy
         */
        virtual void on_destroy_index_buffer(index_buffer* ib);

    public:
        /**
         * @brief Create an index buffer with initial data.
         * @param dt            index buffer data type
         * @param data_size     size in bytes
         * @param data          initial data
         * @return created index buffer handle
         */
        index_buffer_handle create_index_buffer(data_type         dt,
                                                size_t            data_size,
                                                const buffer_ref& data);

        /**
         * @brief Create an empty index buffer.
         * Requires set_data() to be called before use.
         * @param dt            index buffer data type
         * @param data_size     size in bytes
         * @return created index buffer handle
         */
        index_buffer_handle create_index_buffer(data_type dt, size_t data_size);

    protected:
        /**
         * @brief Create a vertex buffer object.
         *
         * @param layout    vertex buffer layout
         * @param data_size size in bytes
         * @return created vertex buffer
         */
        virtual vertex_buffer*
        on_create_vertex_buffer(const vertex_layout& layout,
                                size_t               data_size) = 0;

        /**
         * @brief Destroy a vertex buffer.
         * @param vb vertex buffer to destroy
         */
        virtual void on_destroy_vertex_buffer(vertex_buffer* vb);

    public:
        /**
         * @brief Create a vertex buffer object with initial data.
         * @param layout    vertex buffer layout
         * @param data_size size in bytes
         * @param data      initial data
         * @return created vertex buffer handle
         */
        vertex_buffer_handle create_vertex_buffer(const vertex_layout& layout,
                                                  size_t            data_size,
                                                  const buffer_ref& data);

        /**
         * @brief Create an empty vertex buffer.
         * Requires set_data() to be called before use.
         * @param layout    vertex buffer layout
         * @param data_size size in bytes
         * @return created vertex buffer handle
         */
        vertex_buffer_handle create_vertex_buffer(const vertex_layout& layout,
                                                  size_t data_size);

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
        virtual void on_destroy_shader(shader* s);

        /**
         * @brief Create a program object.
         *
         * @return created program
         */
        virtual program* on_create_program() = 0;
        /**
         * @brief Destroy a program.
         * @param p program to destroy
         */
        virtual void on_destroy_program(program* p);

        virtual frame_buffer* on_create_frame_buffer();
        virtual void          on_destroy_frame_buffer(frame_buffer* fb);

    public:
        /**
         * @brief Create a shader object.
         * @param t             shader type
         */
        shader_handle create_shader(shader_type t);

        /**
         * @brief Create a program object.
         */
        program_handle create_program();

        /**
         * @brief Create a frame buffer object.
         *
         * @return created frame buffer
         */
        frame_buffer_handle create_frame_buffer();

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

        /**
         * @brief Get the default viewport for this render context.
         *
         * @return default viewport
         */
        mge::viewport default_viewport() const;

        /**
         * @brief Get the default scissor rectangle for this render context.
         *
         * @return default scissor rectangle
         */
        mge::rectangle default_scissor() const;

        /**
         * @brief Get a pass by its index. If the pass does not exist, it is
         * created.
         * @param index pass index
         * @return reference to pass
         */
        mge::pass& pass(uint32_t index);

        /**
         * @brief Get the current command buffer.
         * @param clear if true, the command buffer is cleared before returning
         * @return command buffer
         */
        mge::command_buffer& command_buffer(bool clear = false);

        /**
         * @brief Render a frame.
         *
         * This method does the following to process a frame:
         * - Calls all registered prepare frame actions.
         * - Draws all active passes in order.
         * - If any pass was active, presents the swap chain.
         *
         */
        void frame();

    protected:
        /**
         * @brief Called when a frame is being presented.
         */
        virtual void on_frame_present() = 0;

        /**
         * @brief Render a pass.
         * @param p pass to render
         */
        virtual void render(const mge::pass& p);

    public:
        /**
         * @brief Take a screenshot of the current frame buffer.
         *
         * @return image containing the screenshot
         */
        virtual image_ref screenshot() = 0;

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
            } else if constexpr (std::is_same_v<T, index_buffer>) {
                if (object_index < m_index_buffers.size()) {
                    return m_index_buffers[object_index];
                }
            } else if constexpr (std::is_same_v<T, vertex_buffer>) {
                if (object_index < m_vertex_buffers.size()) {
                    return static_cast<vertex_buffer*>(
                        m_vertex_buffers[object_index]);
                }
            }
            return nullptr;
        }

        /**
         * @brief Destroy a context object by type and index.
         *
         * @tparam T object type (shader, program, etc.)
         * @param index object type index
         * @param flags object flags
         * @param object_index index into the object storage
         */
        template <typename T>
        void
        destroy_object(uint16_t index, uint16_t flags, uint32_t object_index)
        {
            if constexpr (std::is_same_v<T, shader>) {
                if (object_index < m_shaders.size()) {
                    on_destroy_shader(m_shaders[object_index]);
                    m_shaders[object_index] = nullptr;
                }
            } else if constexpr (std::is_same_v<T, program>) {
                if (object_index < m_programs.size()) {
                    on_destroy_program(m_programs[object_index]);
                    m_programs[object_index] = nullptr;
                }
            } else if constexpr (std::is_same_v<T, index_buffer>) {
                if (object_index < m_index_buffers.size()) {
                    on_destroy_index_buffer(m_index_buffers[object_index]);
                    m_index_buffers[object_index] = nullptr;
                }
            } else if constexpr (std::is_same_v<T, vertex_buffer>) {
                if (object_index < m_vertex_buffers.size()) {
                    on_destroy_vertex_buffer(static_cast<vertex_buffer*>(
                        m_vertex_buffers[object_index]));
                    m_vertex_buffers[object_index] = nullptr;
                }
            } else if constexpr (std::is_same_v<T, frame_buffer>) {
                if (object_index < m_frame_buffers.size()) {
                    on_destroy_frame_buffer(m_frame_buffers[object_index]);
                    m_frame_buffers[object_index] = nullptr;
                }
            }
        }

        /**
         * @brief Prepare the frame by registering an action to be called
         * before rendering the frame.
         *
         * @param action action to be called
         */
        template <mge::callable ActionType>
        void prepare_frame(ActionType&& action)
        {
            m_prepare_frame_actions.emplace_back(
                std::forward<ActionType>(action));
        }

    protected:
        mge::render_system& m_render_system;
        mge::extent         m_extent;
        uint16_t            m_index{0xFFFF}; //!< index in registry

        std::vector<shader*>        m_shaders;
        std::vector<program*>       m_programs;
        std::vector<index_buffer*>  m_index_buffers;
        std::vector<vertex_buffer*> m_vertex_buffers;
        std::vector<frame_buffer*>  m_frame_buffers;

        using prepare_frame_action = std::function<void()>;

        std::array<std::byte, 65536>           m_prepare_frame_memory;
        std::pmr::monotonic_buffer_resource    m_prepare_frame_resource;
        std::pmr::vector<prepare_frame_action> m_prepare_frame_actions;

        std::vector<mge::pass>               m_passes;
        std::unique_ptr<mge::command_buffer> m_command_buffer;

        bool m_record_frames{false};
        bool m_first_frame{true};
    };

} // namespace mge