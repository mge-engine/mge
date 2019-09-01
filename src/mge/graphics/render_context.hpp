// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/buffer_access.hpp"
#include "mge/graphics/buffer_change_policy.hpp"
#include "mge/core/async_executor.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/shader_language.hpp"
#include "mge/graphics/shader_type.hpp"
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/vertex_layout.hpp"
#include "mge/graphics/texture_type.hpp"
#include "mge/graphics/filter_function.hpp"
#include "mge/core/to_void_ptr.hpp"

#include <memory>
#include <vector>
#include <array>

namespace mge {

    /**
     * A render context is a context in which objects that are elements
     * of the render pipeline are created - such as shader programs,
     * textures and buffers.
     */
    class MGE_GRAPHICS_EXPORT render_context
            : public std::enable_shared_from_this<render_context>
            , public async_executor
    {
    protected:
        render_context(async_executor *display_executor);
    public:
        virtual ~render_context() = default;
        void await(const std::function<void()>& f) override;

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

        virtual vertex_buffer_ref create_vertex_buffer(const vertex_layout& layout,
                                                       buffer_change_policy change_policy,
                                                       buffer_access cpu_access,
                                                       buffer_access gpu_access,
                                                       size_t element_count,
                                                       void *initial_data=nullptr) = 0;

        /**
         * Create an index (element array) buffer.
         * @param type          data type of buffer elements
         * @param change_policy buffer change_policy
         * @param cpu_access    cpu access flags
         * @param gpu_access    gpu access flags
         * @return index buffer
         */
        virtual index_buffer_ref create_index_buffer(data_type type,
                                                     buffer_change_policy change_policy,
                                                     buffer_access cpu_access,
                                                     buffer_access gpu_access,
                                                     size_t element_count,
                                                     void *initial_data=nullptr) = 0;

        /**
         * @reate an index buffer.
         * 
         * @tparam T            type of array values
         * @tparam N            number of array elements
         * @param change_policy buffer change policy
         * @param cpu_access    cpu access flags
         * @param gpu_access    gpu access flags
         * @param buffer        buffer data
         * @return index buffer populated with the contents of @c buffer.
         */
        template <typename T, std::size_t N>
        index_buffer_ref create_index_buffer(buffer_change_policy change_policy,
                                             buffer_access cpu_access,
                                             buffer_access gpu_access,
                                             const std::array<T, N>& buffer)
        {
            return create_index_buffer(data_type_of_type<T>(),
                                       change_policy,
                                       cpu_access,
                                       gpu_access,
                                       N,
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
        index_buffer_ref create_index_buffer(const std::array<T, N>& buffer)
        {
            return create_index_buffer(data_type_of_type<T>(),
                                       buffer_change_policy::DEFAULT,
                                       buffer_access::WRITE,
                                       buffer_access::READ,
                                       N,
                                       to_void_ptr(buffer.data()));
        }

        /**
         * Create a texture object.
         * 
         * @param type          texture type
         * @param mag_filter    magnify filter function
         * @param min_filter    minimize filter function
         * @param mipmap_use    usage of mipmaps
         * @return created texture
         */
        virtual texture_ref create_texture(texture_type type,
                                           filter_function mag_filter,
                                           filter_function min_filter,
                                           bool mipmap_use) = 0;

        /**
         * Create a shader object.
         * 
         * @param type shader type
         * @return created shader
         */
        virtual shader_ref create_shader(shader_type type) = 0;

        /**
         * Create a pipeline object.
         * 
         * @return created pipeline
         */
        virtual pipeline_ref create_pipeline() = 0;

        /**
         * Get supported shader languages.
         * @param languages vector of languages
         */
        virtual void shader_languages(std::vector<shader_language>& languages) const = 0;

    protected:
        async_executor *m_display_executor;
    };
}
