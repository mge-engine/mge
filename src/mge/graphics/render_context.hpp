// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/async_executor.hpp"
#include "mge/core/to_void_ptr.hpp"
#include "mge/entity/entity_registry.hpp"
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/filter_function.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/shader_language.hpp"
#include "mge/graphics/shader_type.hpp"
#include "mge/graphics/texture_type.hpp"
#include "mge/graphics/usage.hpp"
#include "mge/graphics/vertex_layout.hpp"

#include <array>
#include <memory>
#include <string_view>
#include <vector>

namespace mge {

    /**
     * A render context is a context in which objects that are elements
     * of the render pipeline are created - such as shader programs,
     * textures and buffers.
     */
    class MGE_GRAPHICS_EXPORT render_context
        : public std::enable_shared_from_this<render_context>,
          public async_executor
    {
    protected:
        /**
         * Create a new render context that utilizes a given
         * executor.
         *
         * @param system render system
         * @param display_executor context executor
         */
        render_context(render_system *system, async_executor *display_executor);

    public:
        /**
         * Destructor.
         */
        virtual ~render_context() = default;

        /**
         * Executes a function in the context of the render context's
         * asynchronous executor.
         *
         * @param f function to execute
         */
        void await(const void_function &f) override;

        /**
         * Assigns the render context to this thread, in case of the
         * underlying API needs such an assignment.
         */
        virtual void assign_thread();

        /**
         * Flush outstanding work, and return when work has been
         * presented.
         */
        virtual void flush() = 0;

        /**
         * Create a vertex buffer.
         * @param layout        vertex buffer layout
         * @param buffer_usage  buffer usage hint
         * @param element_count number of elements
         * @param initial_data  initial buffer data
         * @return created vertex buffer
         */
        virtual vertex_buffer_ref
        create_vertex_buffer(const vertex_layout &layout, usage buffer_usage,
                             size_t element_count,
                             void * initial_data = nullptr) = 0;

        /**
         * Create an index (element array) buffer.
         * @param type          data type of buffer elements
         * @param buffer_usage  buffer usage
         * @param cpu_access    cpu access flags
         * @param gpu_access    gpu access flags
         * @return index buffer
         */
        virtual index_buffer_ref
        create_index_buffer(data_type type, usage buffer_usage,
                            size_t element_count,
                            void * initial_data = nullptr) = 0;

        /**
         * @reate an index buffer.
         *
         * @tparam T            type of array values
         * @tparam N            number of array elements
         * @param usage buffer change policy
         * @param cpu_access    cpu access flags
         * @param gpu_access    gpu access flags
         * @param buffer        buffer data
         * @return index buffer populated with the contents of @c buffer.
         */
        template <typename T, std::size_t N>
        index_buffer_ref create_index_buffer(usage                   usage,
                                             const std::array<T, N> &buffer)
        {
            return create_index_buffer(data_type_of_type<T>(), usage, N,
                                       to_void_ptr(buffer.data()));
        }

        /**
         * Create a index buffer object from an array of values.
         *
         * @tparam T        type of array values
         * @tparam N        number of array elements
         * @param buffer    content of index buffer
         * @return index buffer populated with the contents of @c buffer.
         */
        template <typename T, std::size_t N>
        index_buffer_ref create_index_buffer(const std::array<T, N> &buffer)
        {
            return create_index_buffer(data_type_of_type<T>(), usage::DEFAULT,
                                       N, to_void_ptr(buffer.data()));
        }

        /**
         * Create a texture object
         *
         * @param image texture image
         * @return created texture
         */
        virtual texture_2d_ref create_texture_2d(const image_ref &image) = 0;

        /**
         * Create a dynamic texture object
         * @return created texture
         */
        virtual texture_2d_ref create_texture_2d() = 0;

        /**
         * Create a shader object.
         *
         * @param type shader type
         * @return created shader
         */
        virtual shader_program_ref create_shader_program(shader_type type) = 0;

        /**
         * @brief Create a shader progam object
         *
         * @param type          shader type
         * @param asset_name
         * @return shader_program_ref
         */
        shader_program_ref
        create_shader_program(shader_type            type,
                              const std::string_view asset_name);

        /**
         * Create a pipeline object.
         *
         * @return created pipeline
         */
        virtual pipeline_ref create_pipeline() = 0;

        /**
         * Create a command list native to this kind of render context.
         *
         * @return created command list
         */
        virtual command_list_ref create_command_list() = 0;

        /**
         * Get supported shader languages.
         * @param languages vector of languages
         */
        virtual void
        shader_languages(std::vector<shader_language> &languages) const = 0;

        /**
         * Executes a command list.
         *
         * @param commands command list with commands to execute
         */
        virtual void execute(const command_list_ref &commands) = 0;

        /**
         * @brief Render system of context,
         *
         * @return render system of context
         */
        mge::render_system &render_system() noexcept
        {
            return *m_render_system;
        }

    protected:
        mge::render_system *m_render_system;
        async_executor *    m_display_executor;
    };
} // namespace mge
